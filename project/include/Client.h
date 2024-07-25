#pragma once
#include <string>
#include <print>
#include <iostream>
#include <optional>
#include <expected>

#include <boost/json.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// P1: Clients accounts
class Auth
{
public:
    Auth() = default;

    static auto work(const std::string& choice) -> std::string;

    static auto validate_response(const std::string_view& response) -> std::optional<bool>;
};

// P2
class TInterfaces
{
public:
    TInterfaces() = default;

    auto work() -> std::optional<std::string>;

    auto validate_response(const std::string_view& response) -> void;

private:
    auto view_market() -> std::string;

    auto form_order() -> std::string;
};

// P0
class Client
{
public:
    Client()
    {
        try
        {
            socket.connect(tcp::endpoint({}, 55555));
        }
        catch (...) { throw; }
    };

    auto run() -> void;

private:
    boost::asio::io_context io_context;
    tcp::socket socket{io_context};
    TInterfaces interfaces;

    std::array<char, 1024> buf;
    boost::system::error_code error;

    std::string login{};
    std::string password{};

    auto send_to_exchange(const std::string& request) -> std::string_view;
};