#include <UnitTests.hpp>
#include "Helpers.h"
#include <cmath>

int main() {
    UnitTests testSuite;

    testSuite.addTest("Dummy_test1",
    [](){
        // Build:
        double value = 0.0;
    
        // Operate:
        auto result = dummyFunc(value);

        // Check:
        ASSERT_NEQ(result, 0.4)
    });

    testSuite.addTest("Test_negative_val",
    []() -> void {
        double value = -10.0;

        auto result = dummyFunc(value);

        ASSERT_EQ(result, 0.0)
    });

    testSuite.addTest("Test_equal_positive",
    []() -> void {
        double value = 10.0;

        auto result = dummyFunc(value);

        ASSERT_EQ(result, 10.0)
    });

    // Tasks #3 start after

    testSuite.addTest("Test_equal_strings",
    []() -> void {
        std::string value = "hello";
        std::vector<int> vec = {0, 2, 1, 0, 3};

        auto result = dummyClassTask(vec, value);

        ASSERT_EQ(result, "hellohellohello")
    });

    testSuite.addTest("Test_empty_vec",
    []() -> void {
        std::string value = "hello";
        std::vector<int> vec;

        auto result = dummyClassTask(vec, value);

        ASSERT_EQ(result, "")
    });

    testSuite.addTest("Test_empty_string",
    []() -> void {
        std::string value = "";
        std::vector<int> vec = {1, 2, 3};

        auto result = dummyClassTask(vec, value);

        ASSERT_EQ(result, "")
    });

    testSuite.addTest("Test_zero_times",
    []() -> void {
        std::string value = "hello";
        std::vector<int> vec = {0};

        auto result = dummyClassTask(vec, value);

        ASSERT_EQ(result, "")
    });

    testSuite.addTest("Test_equal_integers",
    []() -> void {
        std::string value = "hello";
        std::vector<int> vec = {2, 2, 2};

        auto result = dummyClassTask(vec, value);

        ASSERT_EQ(result, "hellohello")
    });

    testSuite.addTest("Test_home_0_product_sum_exist",
    []() -> void {
        auto result = dummyHomeTask(0, 1, 1);

        ASSERT_EQ(result, 0.5)
    });

    testSuite.addTest("Test_home_0_product_0_sum",
    []() -> void {
        auto result = dummyHomeTask(-1, 0, 0);

        ASSERT_EQ(result, -1)
    });

    testSuite.addTest("Test_home_product_exist_but_failed_due_to_epsilon",
    []() -> void {
        auto result = dummyHomeTask(1, 2, 3);
        double verify = 0.146666;
        constexpr double eps = 1e-5;
        // AN to SV 05/07/2024:
        // thats how i tried to do it in macros, but not succeeded, the key idea is to change macros a little bit
        // so we firstly check whether 2 supplied types are both doubles, and if they are then calculate difference with epsilon
        // in this case we will be able to evaluate == correctly, but after many tries i failed to implement it in the macros itself
        //
        // so this below is generally what i intended to do there
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

    testSuite.addTest("Test_home_0_product_0_sum_#2",
    []() -> void {
        auto result = dummyHomeTask(1, 0, -1);

        ASSERT_EQ(result, -1)
    });

    testSuite.run();
}
