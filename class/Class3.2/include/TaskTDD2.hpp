#pragma once
#include <cstdint>
#include <tuple>

using color = std::tuple<uint8_t, uint8_t, uint8_t>;

class RGB
{
public:
    RGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}

    auto get_magic_color() -> color;
private:
    uint8_t r{};
    uint8_t g{};
    uint8_t b{};
};