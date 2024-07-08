#include "Builder.hpp"
#include <ranges>
#include <stdexcept>

auto Builder::build_pizza(const std::vector<std::string>& i_list) -> pizza {
    pizza o_pizza{};
    
    for (const auto& i_name : i_list)
    {
        auto it = std::ranges::find_if(ingredients, [&i_name](const auto& ingredient) {
            return ingredient.get_info().first == i_name;
        });

        if (it != ingredients.end()) {
            auto info = it->get_info();
            o_pizza.first.push_back(info.first);
            o_pizza.second += info.second;
        } else {
            throw std::invalid_argument(std::format("One of your ingredients not found: {}. Try again.", i_name));
        }
    }

    if (!o_pizza.first.empty()) return o_pizza;
    else throw std::logic_error("Something went wrong.");
}