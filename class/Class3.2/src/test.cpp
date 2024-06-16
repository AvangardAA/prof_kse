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
        ASSERT_EQ(std::get<0>(result), 0)
        ASSERT_EQ(std::get<1>(result), 0)
        ASSERT_EQ(std::get<2>(result), 0)
    });

    testSuite.addTest("R component incorrect",
    [](){
        // Build:
        RGB rgb(0,100,100);

        // Operate:
        auto result = rgb.get_magic_color();

        // Check:
        ASSERT_NEQ(result, std::make_tuple(255,198,100))
    });

    testSuite.addTest("G component incorrect",
    [](){
        // Build:
        RGB rgb(100,200,100);

        // Operate:
        auto result = rgb.get_magic_color();

        // Check:
        ASSERT_NEQ(result, std::make_tuple(49,142,100))
    });

    testSuite.addTest("B component incorrect",
    [](){
        // Build:
        RGB rgb(100,100,100);

        // Operate:
        auto result = rgb.get_magic_color();

        // Check:
        ASSERT_EQ(std::get<0>(result), 49)
        ASSERT_EQ(std::get<1>(result), 198)
        ASSERT_EQ(std::get<2>(result), 100)
    });

    testSuite.run();
}