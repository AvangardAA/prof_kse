#include "TaskTDD1.hpp"

auto Triangle::get_area() -> double
{
    double s = (a + b + c) / 2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}