#include <Menu.hpp>
#include <Builder.hpp>
#include <Ingredient.hpp>
#include <UnitTests.hpp>
#include <string>

auto main() -> int 
{
    UnitTests testSuite;

    testSuite.addTest("1 Ingredient creation test",
    [](){
        // Build:
        Ingredient test("onion", 5);

        // Operate:
        auto result = test.get_info();

        // Check:
        ASSERT_EQ(result.first, "onion")
        ASSERT_EQ(result.second, 5)
    });

    testSuite.addTest("2 Empty builder creation test",
    [](){
        // Build:
        Builder builder;

        // Operate:
        try {
            auto result = builder.build_pizza((std::vector<std::string>()));
        } catch (std::exception& exc) {
            ASSERT_EQ(std::string(exc.what()), "Something went wrong.")
        }
    });

    testSuite.addTest("3 Construct menu with error",
    [](){
        // Build:
        // Please make sure you empty the config file in ./local_files/

        // Operate:
        try {
            Menu menu;
        } catch (std::exception& exc) {
            ASSERT_EQ(std::string(exc.what()), "Critical error, parsing of config file failed. Exiting..")
        }
    });

    testSuite.addTest("4 Construct builder and make pizza with ingredients",
    [](){
        // Build:

        Builder builder(std::vector<Ingredient>({Ingredient("onion", 5), Ingredient("garlic", 10)}));

        // Operate:
        auto result = builder.build_pizza((std::vector<std::string>({"onion", "garlic"})));
        ASSERT_EQ(result.second, 15)
    });

    testSuite.addTest("5 Wrong ingredient for pizza",
    [](){
        // Build:

        Builder builder(std::vector<Ingredient>({Ingredient("onion", 5), Ingredient("garlic", 10)}));

        // Operate:
        try {
            auto result = builder.build_pizza((std::vector<std::string>({"dummy"})));
        } catch (std::exception& exc) {
            ASSERT_EQ(std::string(exc.what()), 
            "One of your ingredients not found: dummy. Try again.")
        }
    });

    testSuite.run();
}