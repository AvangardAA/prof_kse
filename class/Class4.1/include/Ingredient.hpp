#pragma once
#include <string>
#include <utility>

using ingr_info = std::pair<std::string, int>;

class Ingredient 
{
public:
    explicit Ingredient(std::string i_name, int i_cost) : name(std::move(i_name)), cost(i_cost) {}

    [[nodiscard]] auto get_info() const -> ingr_info;
private:
    std::string name;
    int cost;
};