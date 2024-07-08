#include <Ingredient.hpp>
#include <Builder.hpp>
#include <Menu.hpp>

auto main() -> int {
    
    try 
    {
        Menu menu;
        menu.loop();
    } 
    catch (std::exception& exc) {std::cout << exc.what() << std::endl; return 0;}

    return 0;
}
