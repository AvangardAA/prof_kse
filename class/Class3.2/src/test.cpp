#include <print>
#include <TaskTDD1.hpp>
#include <TaskTDD2.hpp>
#include <UnitTests.hpp>

auto main() -> int 
{
    UnitTests testSuite;

    testSuite.addTest("Formula works",
    [](){
        // Build:
        Triangle triangle(3,4,5);

        // Operate:
        auto result = triangle.get_area();

        // Check:
        ASSERT_EQ(result, 6)
    });

    testSuite.addTest("Formula ok, but triangle dont exist",
    [](){
        // Build:
        Triangle triangle(1,1,2);

        // Operate:
        auto result = triangle.get_area();

        // Check:
        ASSERT_NEQ(result, 0)
    });

    testSuite.addTest("Zero lengths",
    [](){
        // Build:
        Triangle triangle(0,0,0);

        // Operate:
        auto result = triangle.get_area();

        // Check:
        ASSERT_NEQ(result, 0)
    });

    // Here we start second task tests

    testSuite.addTest("Input incorrect",
    [](){
        // Build:
        RGB rgb(0,0,0);

        // Operate:
        auto result = rgb.get_magic_color();

        // Check:
        ASSERT_NEQ(std::get<0>(result), 0)
    });

    testSuite.run();
}