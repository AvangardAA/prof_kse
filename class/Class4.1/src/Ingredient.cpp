#include "Ingredient.hpp"

auto Ingredient::get_info() const -> ingr_info {
    return std::make_pair(name, cost);
}