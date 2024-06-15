#include <print>
#include <Helpers.hpp>
#include <UnitTests.hpp>

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

    testSuite.run();
}
