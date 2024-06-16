#include "TaskTDD1.hpp"

auto Triangle::get_area() -> double
{
    if (!(a + b > c && a + c > b && b + c > a)) {return -1;}

    double s = (a + b + c) / 2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}