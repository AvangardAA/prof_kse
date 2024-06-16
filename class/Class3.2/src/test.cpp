#include <print>
#include <TaskTDD1.hpp>
#include <UnitTests.hpp>

auto main() -> int 
{
    UnitTests testSuite;

    testSuite.addTest("TDD",
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
        Triangle triangle(3,4,5);

        // Operate:
        auto result = triangle.get_area();

        // Check:
        ASSERT_NEQ(result, 6)
    });

    testSuite.run();
}