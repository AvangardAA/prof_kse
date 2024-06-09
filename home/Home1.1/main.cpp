#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <vector>
// This program implements user story a) defined in class1.1

auto main() -> int 
{
    std::print("Hello, please input integer values to find biggest negative in one line\n");

    auto input = []() -> std::vector<int> 
    {
        std::string line;
        int num = 0;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::vector<int> values;

        while (iss >> num) 
        {
            values.push_back(num);
        }

        return values;
    };

    std::vector values = input();

    bool hasNegative = std::any_of(values.begin(), values.end(), [](int num) 
    {
        return num < 0;
    });

    if (!hasNegative)
    {
        std::println("No negative values provided. Exit");
        return 0;
    }

    int choice = 1;
    std::println("Choose what's 'biggest'?: Default - closest val. to zero (input 0), largest absolute (input 1): ");
    std::cin >> choice;

    if (choice != 0)
    {
        auto biggest = std::min_element(values.begin(), values.end(),
        [](int a, int b) 
        {
            return a < b && a < 0 && b < 0;
        });

        std::print("Biggest value is: ");
        std::cout << *biggest << std::endl;
    }

    else 
    {
        auto biggest = std::min_element(values.begin(), values.end(), 
        [](int a, int b) 
        {
            return a < 0 && (b >= 0 || a > b);
        });

        std::print("Biggest value is: ");
        std::cout << *biggest << std::endl;
    }
    return 0;
}