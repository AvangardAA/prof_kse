#pragma once
#include <string>
#include <print>
#include <vector>
#include <map>

#include <boost/json.hpp>

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>

#include "Namespaces.hpp"
#include "EncryptionUtils.h"
#include "UserUtils.h"
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

constexpr int start_ids_from{100};

class ExchangeCore 
{
public:
    ExchangeCore() = default;

    auto process_message(boost::json::value jv, int sock) -> std::string_view;

    auto receive_message(boost::asio::mutable_buffer buffer, int sock) -> boost::asio::const_buffer;

    auto run() -> void;

    auto debug_init_matchings(std::vector<std::string> instruments) -> void;
    auto debug_get_matching_inst() -> std::string;

private:
    boost::asio::io_context ctx{1};
    boost::asio::signal_set signals{ctx, SIGINT, SIGTERM};
    CustomTypes::login_encrypted users{};
    int seq_num{start_ids_from};
    std::map<int, std::pair<std::string, std::string>> authorized{};
    UserUtils user_utils;
    std::vector<MatchingEngine> matchings{};

    auto check_existing(std::vector<std::string> fields, boost::json::value jv) -> bool;

    auto init_matchings(std::vector<std::string> instruments) -> void;

    // AN (24/07/2024): Boost.Asio boiler for coroutines:
    // https://www.boost.org/doc/libs/1_85_0/doc/html/boost_asio/example/cpp20/coroutines/echo_server.cpp
    awaitable<void> echo(tcp::socket socket);

    awaitable<void> listener();
};