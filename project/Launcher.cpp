#include "Client.h"
#include <boost/json/src.hpp>

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