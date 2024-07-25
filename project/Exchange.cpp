#include <string>
#include <print>
#include <vector>
#include <map>

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>

#include "EncryptionUtils.h"
#include "UserUtils.h"
#include "Namespaces.hpp"
#include "MatchingEngine.h"

// AN (24/07/2024): Boost.Asio boiler for coroutines:
// https://www.boost.org/doc/libs/1_85_0/doc/html/boost_asio/example/cpp20/coroutines/echo_server.cpp
using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

#if defined(BOOST_ASIO_ENABLE_HANDLER_TRACKING)
# define use_awaitable \
  boost::asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

// P0: Exchange core
class ExchangeCore 
{
public:
  ExchangeCore() = default;

  auto process_message(boost::json::value jv, int sock) -> std::string_view
  {
    std::string request = jv.at("method").as_string().c_str();

    if (request == "auth")
    {
      if (!check_existing({"login", "password"}, jv))
      {
        return Messages::missing_credentials;
      }

      std::string login = jv.at("login").as_string().c_str();
      // TODO (AN): invalid input, empty fields
      if (users.find(login) == users.end()) {return Messages::not_exist;}

      std::string hash = EncryptionUtils().hash_string(jv.at("password").as_string().c_str());

      try 
      {
        std::string test_pwd = EncryptionUtils().decrypt_aes(users[login], hash);
      }
      catch (...) 
      {
        return Messages::auth_failed;
      }

      authorized[sock] = std::make_pair(login, hash);
      return Messages::auth_success;
    }

    if (request == "register")
    {
      if (!check_existing({"login", "password"}, jv))
      {
        return Messages::missing_credentials;
      }

      std::string login = jv.at("login").as_string().c_str();
      // TODO (AN): invalid input, empty fields
      if (users.find(login) != users.end()) {return Messages::already_existing;}

      std::string hash = EncryptionUtils().hash_string(jv.at("password").as_string().c_str());
      boost::json::object obj;
      obj["password"] = hash;
      obj["orders"] = boost::json::array();
      obj["history"] = boost::json::array();
      std::string user_data = boost::json::serialize(obj);

      users[login] = EncryptionUtils().encrypt_aes(user_data, hash);
      user_utils.save_data(users);

      authorized[sock] = std::make_pair(login, hash);
      return Messages::successfully_registered;
    }

    else if (request == "LOB")
    {
      if (authorized[sock].first.empty() && authorized[sock].second.empty()) {return Messages::not_authorized;}

      if (!check_existing({"instrument"}, jv))
      {
        return Messages::wrong_LOB_params;
      }

      for (auto& match : matchings)
      {
        if (match.get_instrument() == std::string(jv.at("instrument").as_string().c_str()))
        {
          std::string_view LOB(match.print_LOB());
          return LOB;
        }
      }
    }

    else if (request == "order")
    {
      if (authorized[sock].first.empty() && authorized[sock].second.empty()) {return Messages::not_authorized;}

      if (!check_existing({"type", "qty", "isBid", "instrument"}, jv))
      {
        return Messages::wrong_order_params;
      }

      for (auto& match : matchings)
      {
        std::string instrument = jv.at("instrument").as_string().c_str();
        if (match.get_instrument() == instrument)
        {
          double qty = jv.at("qty").as_double();
          bool isBid = jv.at("isBid").as_bool();

          if (std::string(jv.at("type").as_string().c_str()) == "limit")
          {
            if (!check_existing({"price"}, jv))
            {
              return Messages::wrong_order_params;
            }

            double price = jv.at("price").as_double();
            match.process_limit(LimitOrder(authorized[sock].first, seq_num, price, qty, isBid));
            seq_num++;

            return Messages::order_submit_success;
          }
          else 
          {
            match.process_market(MarketOrder(authorized[sock].first, seq_num, qty, isBid));
            seq_num++;

            return Messages::order_submit_success;
          }
        }
      }

      return Messages::wrong_order_params;
    }

    else if (request == "cabinet")
    {
      if (authorized[sock].first.empty() && authorized[sock].second.empty()) {return Messages::not_authorized;}

      try 
      {
        std::string user_data = EncryptionUtils().decrypt_aes(users[authorized[sock].first], authorized[sock].second);
        std::string_view prepare_json(user_data);
        boost::json::value jv_data = boost::json::parse(prepare_json);
      }
      catch (...) 
      {
        return Messages::auth_failed;
      }

      if (jv.is_object()) {return Messages::auth_success;}
    }
  }

  auto receive_message(boost::asio::mutable_buffer buffer, int sock) -> boost::asio::const_buffer
  {
    try
    {
      std::string_view json_str(boost::asio::buffer_cast<const char*>(buffer), boost::asio::buffer_size(buffer));
      boost::json::value jv = boost::json::parse(json_str);

      if (jv.is_object())
      {
        if (jv.as_object().find("method") == jv.as_object().end())
        {
          throw;
        }
        else 
        {
          return boost::asio::buffer(process_message(jv, sock));
        }
      }
    } 
    catch (const std::exception& exc) 
    {
      std::println("Json error: {}", exc.what());
      throw;
    }
  }

  auto run() -> void
  {
    signals.async_wait([&](auto, auto){ ctx.stop(); });

    co_spawn(ctx, listener(), detached);

    users = user_utils.load_users();

    init_matchings({"USDUAH", "USDEUR", "USDGBP"});

    ctx.run();
  }

private:
  boost::asio::io_context ctx{1};
  boost::asio::signal_set signals{ctx, SIGINT, SIGTERM};
  CustomTypes::login_encrypted users{};
  int seq_num{100};
  std::map<int, std::pair<std::string, std::string>> authorized{};
  UserUtils user_utils;
  std::vector<MatchingEngine> matchings{};

  auto check_existing(std::vector<std::string> fields, boost::json::value jv) -> bool
  {
    for (const auto& field : fields)
    {
      if (jv.as_object().find(field) == jv.as_object().end())
      {
        return false;
      }
    }

    return true;
  }

  auto init_matchings(std::vector<std::string> instruments) -> void
  {
    for (const auto& inst : instruments)
    {
      matchings.push_back(MatchingEngine(inst));
    }
  }

  // AN (24/07/2024): Boost.Asio boiler for coroutines:
  // https://www.boost.org/doc/libs/1_85_0/doc/html/boost_asio/example/cpp20/coroutines/echo_server.cpp
  awaitable<void> echo(tcp::socket socket)
  {
    boost::asio::ip::tcp::socket::native_handle_type native_socket = socket.native_handle();
    int socket_number = static_cast<int>(native_socket);
    try
    {
      std::array<char, 1024> data;
      for (;;)
      {
        std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data), use_awaitable);
        if (n > 0)
        {
          co_await async_write(socket, receive_message(boost::asio::buffer(data, n), socket_number), use_awaitable);
        }
        // TODO (AN): malformed message
      }
    }
    catch (std::exception& e)
    {
      authorized.erase(socket_number);
      //std::println("Exception: {}", e.what());
    }
  }

  awaitable<void> listener()
  {
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, tcp::endpoint({}, 55555));
    for (;;)
    {
      tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
      boost::asio::ip::tcp::socket::native_handle_type native_socket = socket.native_handle();
      int socket_number = static_cast<int>(native_socket);
      authorized[socket_number] = std::make_pair(std::string(), std::string());
      co_spawn(executor, echo(std::move(socket)), detached);
    }
  }
};

auto main() -> int 
{
  try
  {
      if (sodium_init() < 0) { throw; }

      ExchangeCore exchange;

      exchange.run();
  }
  catch (std::exception& e)
  {
      std::println("Critical error occured: exchange cant run further.");
  }

  return 0;
}