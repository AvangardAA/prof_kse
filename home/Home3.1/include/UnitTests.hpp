#pragma once

#include <vector>
#include <functional>
#include <format>
#include <string>
#include <iostream>
#include <cassert>
#include <source_location>

#define ASSERT_EQ(leftValue, rightValue) \
    if ( leftValue != rightValue ) { \
        auto location = std::source_location::current(); \
        std::cout << std::format("Assert failed: {}(which is equal to {}) is not equal to {} at {}:{}", #leftValue, leftValue, #rightValue, location.file_name(), location.line()) << std::endl; \
        throw std::invalid_argument("Assertion failed"); \
    }

#define ASSERT_NEQ(leftValue, rightValue) \
    if ( leftValue == rightValue ) { \
        auto location = std::source_location::current(); \
        std::cout << std::format("Assert failed: {} is equal to {} at {}:{}", #leftValue, #rightValue, location.file_name(), location.line()) << std::endl; \
        throw std::invalid_argument("Assertion failed"); \
    }

class UnitTests {
public:
    using TestFunc = std::function<void(void)>;

    void addTest(const std::string& name, TestFunc&& func) {
        tests.push_back({name, std::move(func)});
    }

    void run() {
        constexpr auto greenColor = "\033[32m";
        constexpr auto redColor = "\033[31m";
        constexpr auto endColor = "\033[m";

        size_t totalCount = 0;
        size_t successCount = 0;

        for (auto& [name, testFunc] : tests) {
            if (testFunc) {
                std::cout << std::format("Executing test {}...", name) << std::endl;
                totalCount++;
                try {
                    testFunc();
                    std::cout << std::format("{} {}PASSED{}.", name, greenColor, endColor) << std::endl;
                    successCount++;
                } catch (std::exception& e) {
                    std::cout << std::format("{} {}FAILED{}.", name, redColor, endColor) << std::endl;
                }
            }
        }
        std::cout << std::format("{} of {} {} have passed", successCount, totalCount, totalCount == 1 ? "test" : "tests") << std::endl;
    }

private:
    std::vector<std::pair<std::string, TestFunc>> tests;
};
