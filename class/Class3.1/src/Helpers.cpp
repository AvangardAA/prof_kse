#include "Helpers.hpp"

double dummyFunc(double x) {
    return x > 0 ? x : 0;
}

auto dummyClassTask(const std::vector<int>& vec, const std::string& string_to_rep) -> std::string {
    std::string output = "";
    if (vec.empty()) {return output;}

    if (string_to_rep == "") {return output;}

    auto res = std::ranges::max_element(vec.begin(), vec.end());
    for (int i = 0; i < *res; i++) {output += string_to_rep;}
    return output;
}
