#include <string>
#include <print>
#include <vector>

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

// P0: Matching engine
class MatchingEngine 
{
public:
    int dummy{};
};

// P0: Exchange core
class ExchangeCore 
{
public:
  ExchangeCore() = default;

  auto process_message(boost::json::value jv) -> std::string_view
  {
    std::string request = jv.at("method").as_string().c_str();

    if (request == "auth")
    {
      std::string_view res = R"({"Hello":"world"})";
      return res;
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
      std::string user_data = R"({"password":")" + hash + R"(","orders":[],"history":[]})";

      users[login] = EncryptionUtils().encrypt_aes(user_data, hash);
      
      /* DEBUG
      for (const auto& u : users)
      {
        std::println("{},{}", u.first, u.second);
      }
      */
      return Messages::successfully_registered;
    }

    else if (request == "order")
    {
      std::string_view res = R"({"Hello":"world1"})";
      return res;
    }

    else if (request == "trades")
    {
      std::string_view res = R"({"Hello":"world2"})";
      return res;
    }

    else if (request == "book")
    {
      std::string_view res = R"({"Hello":"world3"})";
      return res;
    }
  }

  auto receive_message(boost::asio::mutable_buffer buffer) -> boost::asio::const_buffer
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
          return boost::asio::buffer(process_message(jv));
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

    ctx.run();
  }

private:
  boost::asio::io_context ctx{1};
  boost::asio::signal_set signals{ctx, SIGINT, SIGTERM};
  CustomTypes::login_encrypted users{};
  UserUtils user_utils;

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

  // AN (24/07/2024): Boost.Asio boiler for coroutines:
  // https://www.boost.org/doc/libs/1_85_0/doc/html/boost_asio/example/cpp20/coroutines/echo_server.cpp
  awaitable<void> echo(tcp::socket socket)
  {
    try
    {
      std::array<char, 1024> data;
      for (;;)
      {
        std::size_t n = co_await socket.async_read_some(boost::asio::buffer(data), use_awaitable);
        if (n > 0)
        {
          co_await async_write(socket, receive_message(boost::asio::buffer(data, n)), use_awaitable);
        }
        // TODO (AN): malformed message
      }
    }
    catch (std::exception& e)
    {
      std::println("Exception: {}", e.what());
    }
  }

  awaitable<void> listener()
  {
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, tcp::endpoint({}, 55555));
    for (;;)
    {
      tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
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
      std::println("Critical error occured: ", e.what());
  }

  return 0;
}