#pragma once

#include "Ingredient.hpp"
#include "Builder.hpp"
#include <print>
#include <fstream>
#include <ios>
#include <limits>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <string_view>

class Menu 
{
public:
    Menu() {
        load_ingredients();
        builder = Builder(inventory);
    }

    auto loop() -> void;

    auto print_ingredients() -> void;

    auto input_ingredients() -> void;

private:
    std::vector<Ingredient> inventory;
    Builder builder;
    auto load_ingredients() -> void;
};