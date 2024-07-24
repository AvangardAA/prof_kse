#include <string>
#include <print>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// P2: History and orders menu
class UInfo
{

};

// P2: Trading interfaces
class TInterfaces
{

};

// P1: Clients accounts
class Auth
{

};

// P0: Trading client
class Client
{

};

auto main() -> int 
{
    try 
    {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);
        socket.connect(tcp::endpoint({}, 55555));

        std::string message = R"({"method":"order"})";
        boost::asio::write(socket, boost::asio::buffer(message));

        std::array<char, 1024> buf;
        boost::system::error_code error;
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if (error == boost::asio::error::eof) { std::println("Connection closed by exchange"); } 
        else if (error) { throw boost::system::system_error(error); }

        boost::asio::mutable_buffer msg = boost::asio::buffer(buf, len);
        std::string_view json_str(boost::asio::buffer_cast<const char*>(msg),
                                    boost::asio::buffer_size(msg));

        std::println("{}", std::string(json_str));
    } 
    catch (const std::exception& exc)
    {
        std::println("{}", exc.what());
    }

    return 0;
}