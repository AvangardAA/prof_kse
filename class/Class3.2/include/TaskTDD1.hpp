#pragma once

class Triangle
{
public:
    Triangle(int a, int b, int c) : a(a), b(b), c(c) {}

    auto get_area() -> double;

private:
    int a{};
    int b{};
    int c{};
};