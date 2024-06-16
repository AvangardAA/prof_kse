#include "TaskTDD2.hpp"

auto RGB::get_magic_color() -> color
{
    return std::make_tuple((r / 2) - 1, (g * 2) - 2, b);
}