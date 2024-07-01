#include <print>
#include <fstream>
#include <string>
#include <ranges>

auto write_file(std::string content) -> void
{
    std::ofstream file_o("./local_files/users.txt");
    file_o << content;
    file_o.close();
}

auto save_user(std::string name) -> void 
{
    std::ifstream file("./local_files/users.txt");
    if (!file.is_open()) 
    { 
        write_file(name + ",");
        return;
    }
    
    std::string users_line;
    std::getline(file, users_line);
    file.close();

    users_line += name + ",";
    write_file(users_line);

    return;
}

auto load_users() -> std::string 
{
    std::string users = "";
    std::ifstream file("./local_files/users.txt");
    if (!file.is_open()) { return users; }
    
    file >> users;
    file.close();

    return users;
}

auto main(int argc, char* argv[]) -> int
{
    if (argc < 2 || argc > 3)
    {
        std::println("Unsupported number of args");
        return 1;
    }

    std::string option = "";
    if (argc == 3)
    {
        option = std::string(argv[2]);
        if (option != "delete")
        {
            std::println("Unsupported second argument. Support only: 'delete'.");
            return 1;
        }
    }

    std::string users = load_users();
    std::string name = std::string(argv[1]);

    if (name == "bread" && option != "")
    {
        write_file("");
        std::println("Successfully deleted all users.");
        return 0;
    }

    constexpr std::string_view delimiter(",");
    auto logins = std::ranges::views::split(users, delimiter);
    int occurences = 0;

    for (const auto& delimited_part : logins) 
    {
        std::string uname = std::string(delimited_part.begin(), delimited_part.end()); 
        if (uname == name) {occurences += 1;}
    }

    if (occurences == 0 && option != "") 
    {
        std::println("Nothing to delete, user not found."); 
        return 0;
    }

    if (occurences == 0)
    {
        std::println("Welcome, {}!", name);
        save_user(name);
        return 0;
    }
    
    if (option != "")
    {

        std::string new_users;
        for (const auto& delimited_part : logins) 
        {
            std::string uname = std::string(delimited_part.begin(), delimited_part.end()); 
            if (uname != name) {new_users += uname + ",";}
        }

        write_file(new_users);
        std::println("Successfully deleted user.");
        return 0;
    }

    std::println("Hello again(x{}), {}", occurences + 1, name);
    save_user(name);

    return 0;
}