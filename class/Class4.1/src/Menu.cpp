#include "Menu.hpp"

constexpr std::string_view config("./local_files/inventory.txt");

auto Menu::load_ingredients() -> void {
    std::ifstream file((std::string(config)));
    if (!file.is_open())
    {
        throw std::logic_error("Critical error, there is no config for ingredients. Exiting..");
    }
    
    std::string ingredient{};
    while (std::getline(file, ingredient))
    {
        std::istringstream iss(ingredient);
        std::string i_name{};
        int cost{};

        if (std::getline(iss, i_name, ',') && (iss >> cost))
        {
            inventory.push_back(Ingredient(i_name, cost));
        }
        else
        {
            file.close();
            throw std::logic_error("Critical error, parsing of config file failed. Exiting..");
        }
    }
}

auto Menu::input_ingredients() -> void {
    std::string line{};
    std::string ingredient{};

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::vector<std::string> i_ingredients;

    while (iss >> ingredient) 
    {
        i_ingredients.push_back(ingredient);
    }

    try {
        auto result = builder.build_pizza(i_ingredients);
        std::println("\nYour pizza ready and the cost is {}\nIt consists of:", result.second);

        int counter = 1;
        for (const auto& ingredient : result.first) {std::println("{} : {}", counter, ingredient); counter++;}
    }
    catch (std::exception& exc) {std::cout << exc.what() << std::endl;}
}

auto Menu::print_ingredients() -> void {
    std::println("\nAvailable ingredients and their cost below in form <name> : <cost>");

    for (const auto& ingredient : inventory)
    {
        auto info = ingredient.get_info();
        std::println("{} : {}", info.first, info.second);
    }
}

auto Menu::loop() -> void {
    while (true)
    {
        std::string choice{};
        std::println("\nProceed with: 'help' - list ingredients, 'build' - build pizza. End with 'exit'");
        std::cin >> choice;

        if (choice == "help") print_ingredients();
        else if (choice == "build") input_ingredients();
        else if (choice == "exit") return;
    }
}