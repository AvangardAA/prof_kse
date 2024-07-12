#pragma once

#include "Ingredient.hpp"
#include <vector>
#include <format>

using pizza = std::pair<std::vector<std::string>, int>;

class Builder 
{
public:
    Builder() = default;

    explicit Builder(const std::vector<Ingredient>& i_ingredients)
     : ingredients(i_ingredients) {}

    auto build_pizza(const std::vector<std::string>& i_list) -> pizza;
private:
    std::vector<Ingredient> ingredients;
};