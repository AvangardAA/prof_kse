#include <UnitTests.hpp>
#include "TaskTDD1.h"
#include "TaskTDD2.h"

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

    testSuite.addTest("Invalid side for height",
    [](){
        // Build:
        Triangle triangle(3,4,5);
        
        // Operate:
        auto result = triangle.get_height("hx");
        
        // Check:
        ASSERT_EQ(result, 0)
    });

    testSuite.addTest("Invalid height",
    [](){
        // Build:
        Triangle triangle(3,4,5);
        
        // Operate:
        auto result = triangle.get_height("h_a");
        
        // Check:
        ASSERT_EQ(result, 4)
    });

    testSuite.addTest("Triangle not right",
    [](){
        // Build:
        Triangle triangle(3,3,5);
        
        // Operate:
        auto result = triangle.get_height("h_a");
        
        double verify = 2.76384;
        constexpr double eps = 1e-4;

        if (std::is_same<decltype(result), double>::value == std::is_same<decltype(verify), double>::value)
        {
            if (std::abs(result - verify) > eps)
            {
                std::string res2 = "";
                ASSERT_EQ(res2, "see code @ line 104, explanation there")
            }

            else
            {
                ASSERT_EQ(1, 1)
            }
        }
    });

    testSuite.addTest("Another triangle not right",
    [](){
        // Build:
        Triangle triangle(2,6,5);
        
        // Operate:
        auto result = triangle.get_height("h_b");
        
        double verify = 1.56125;
        constexpr double eps = 1e-4;

        if (std::is_same<decltype(result), double>::value == std::is_same<decltype(verify), double>::value)
        {
            if (std::abs(result - verify) > eps)
            {
                std::string res2 = "";
                ASSERT_EQ(res2, "see code @ line 104, explanation there")
            }

            else
            {
                ASSERT_EQ(1, 1)
            }
        }
    });

    testSuite.run();
}