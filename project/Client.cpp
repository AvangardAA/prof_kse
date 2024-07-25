#include <string>
#include <print>
#include <iostream>
#include <optional>
#include <expected>
#include <boost/json.hpp>
#include <boost/json/src.hpp>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// P2: Trading interfaces

class TInterfaces
{
public:
    TInterfaces() = default;

    auto work() -> std::optional<std::string>
    {
        std::print("\nPick desired operation:\nVisit cabinet: (1)\nView Market: (2)\nTrade: (3)\nInput option number: => ");
        int choice{};
        std::cin.clear();
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                return std::string(R"({"method":"cabinet"})");
            case 2:
                return view_market();
            case 3:
                return form_order();
            default:
                std::println("\nCommand you gave is not supported, restart the application.");
                return std::nullopt;
        }
    }

    auto validate_response(const std::string_view& response) -> void
    {
        std::println("{}", response);
    }

private:
    auto view_market() -> std::string
    {
        std::string instrument{};

        std::print("\nInput instrument ('USDUAH' | 'USDEUR' | 'USDGBP'): => ");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, instrument);

        std::string message = R"({"method":"LOB","instrument":")" + instrument + R"("})";
        return message;
    }

    auto form_order() -> std::string
    {
        std::string type{};
        std::string instrument{};
        std::print("\nInput order type (market/limit): => ");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, type);

        std::print("\nInput instrument ('USDUAH' | 'USDEUR' | 'USDGBP'): => ");
        std::cin.clear();
        std::cin >> instrument;
        
        std::print("\nInput quantity: => ");
        double qty{};
        std::cin.clear();
        std::cin >> qty;

        std::print("\nInput side (1 for buy, 0 for sell): => ");
        bool isBid{};
        std::cin.clear();
        std::cin >> isBid;

        if (type == "market")
        {
            std::string message = R"({"method":"order","instrument":")" + instrument + R"(","type":")" + \
                type + R"(","qty":)" + std::to_string(qty) + R"(,"isBid":)" + (isBid ? "true" : "false") + R"(})";
            
            std::println("{}", message);
            return message;
        }

        else 
        {
            std::print("\nInput price: => ");
            double price{};
            std::cin.clear();
            std::cin >> price;

            std::string message = R"({"method":"order","instrument":")" + instrument + R"(","type":")" + \
                type + R"(","qty":)" + std::to_string(qty) + R"(,"isBid":)" + (isBid ? "true" : "false") + \
                R"(,"price":)" + std::to_string(price) + R"(})";
            
            return message;
        }
    }
};

// P1: Clients accounts
class Auth
{
public:
    Auth() = default;

    static auto work(const std::string& choice) -> std::string
    {
        std::string login{};
        std::string password{};

        if (choice == "register")
        {
            std::print("\nPlease input a login you want to use: ");
            std::getline(std::cin, login);

            std::print("\nPlease input a password you want to use: ");
            std::getline(std::cin, password);

            std::string message = R"({"method":"register","login":")" + login + R"(","password":")" + password + R"("})";
            return message;
        }

        else if (choice == "auth")
        {
            std::print("\nPlease input your login: ");
            std::getline(std::cin, login);

            std::print("\nPlease input your password: ");
            std::getline(std::cin, password);

            std::string message = R"({"method":"auth","login":")" + login + R"(","password":")" + password + R"("})";
            return message;
        }

        else { return std::string("abort"); }
    }

    static auto validate_response(const std::string_view& response) -> std::optional<bool>
    {
        if (response.empty()) {std::println("\nCommand you gave is not supported, restart the application."); return std::nullopt;}
        
        boost::json::value jv = boost::json::parse(response);

        if (std::string(jv.at("status").as_string().c_str()) != "ok")
        {
            std::println("Register or auth went wrong because: {}", jv.at("msg").as_string().c_str());
            return std::nullopt;
        }

        return true;
    }
};

// P0: Trading client
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

    auto run() -> void
    {
        std::string choice{};
        std::println("Welcome to dummy exchange!\n\nPlease authorize or register in system (or exit via 'exit')\n(type register or auth): => ");
        std::getline(std::cin, choice);

        if (choice == "exit") { std::println("Goodbye!"); return; }

        else
        {
            // AN (24/07/2024): if user cant continue, means already registered or wrong credentials - exit app.
            std::optional<bool> response = Auth().validate_response(send_to_exchange(Auth().work(choice)));
            if (response.has_value())
            {
                if (response.value()) {std::println("You are authorized."); }
            }
            else
            {
                return; 
            }
        }

        while (true)
        {
            std::optional<std::string> cmd = interfaces.work();

            if (cmd.has_value()) { interfaces.validate_response(send_to_exchange(cmd.value())); }
            else { break; }
        }
    }

private:
    boost::asio::io_context io_context;
    tcp::socket socket{io_context};
    TInterfaces interfaces;

    std::array<char, 1024> buf;
    boost::system::error_code error;

    std::string login{};
    std::string password{};

    auto send_to_exchange(const std::string& request) -> std::string_view
    {
        try
        {
            if (request == "abort") { return std::string_view(); }

            boost::asio::write(socket, boost::asio::buffer(request));
            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof) { std::println("Connection closed by exchange"); } 
            else if (error) { throw boost::system::system_error(error); }

            boost::asio::mutable_buffer msg = boost::asio::buffer(buf, len);
            std::string_view json_str(boost::asio::buffer_cast<const char*>(msg),
                                        boost::asio::buffer_size(msg));

            return json_str;
        }
        catch (const std::exception& exc) { throw; }
    }
};

auto main() -> int 
{
    try 
    {
        Client client;
        client.run();
    }
    catch (const std::exception& exc) { std::println("Critical error occured: client cant continue."); }

    return 0;
}