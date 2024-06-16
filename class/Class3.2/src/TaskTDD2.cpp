#include "TaskTDD2.hpp"

auto RGB::get_magic_color() -> color
{
    if ((r / 2) - 1 < 0) {return std::make_tuple(0,0,0);}
    if ((g * 2) - 2 > 255) {return std::make_tuple(0,0,0);}

    return std::make_tuple((r / 2) - 1, (g * 2) - 2, b);
}