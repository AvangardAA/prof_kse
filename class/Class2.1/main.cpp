#include <print>
#include <fstream>
#include <string>
#include <ranges>
#include <optional>

using name_option = std::pair<std::string, std::string>;

constexpr std::string_view delimiter(",");
constexpr std::string_view delete_cmd("delete");
constexpr std::string_view bread_key("bread");
constexpr std::string_view clear_file("");

struct UserStatistics 
{
    UserStatistics(const std::string& fname) : filename(fname) {}

    std::string filename{};

    auto load_users() -> std::string;
    auto save_user(const std::string& name) -> void;
    auto write_file(const std::string& content) -> void;
    auto count_logins(const std::string name) -> int;
};

auto UserStatistics::load_users() -> std::string 
{
    std::string users{};
    std::ifstream file(filename);
    if (!file.is_open()) { return users; }
    
    file >> users;
    file.close();

    return users;
}

auto UserStatistics::save_user(const std::string& name) -> void 
{
    std::ifstream file(filename);
    if (!file.is_open()) 
    { 
        write_file(name + std::string(delimiter));
        return;
    }
    
    std::string users_line;
    std::getline(file, users_line);
    file.close();

    users_line += name + std::string(delimiter);
    write_file(users_line);

    return;
}

auto UserStatistics::write_file(const std::string& content) -> void
{
    std::ofstream file_o(filename);
    file_o << content;
    file_o.close();
}

auto validate_input(int argc, char* argv[]) -> std::optional<name_option>
{
    if (argc < 2 || argc > 3)
    {
        std::println("Unsupported number of args");
        exit(1);
    }
    
    std::string name = std::string(argv[1]);
    if (argc == 3)
    {
        std::string option{argv[2]};
        if (option != delete_cmd)
        {
            std::println("Unsupported second argument. Support only: 'delete'.");
            exit(1);
        }

        return name_option(name, option);
    }

    return std::nullopt;
}

auto main(int argc, char* argv[]) -> int
{
    UserStatistics user_stats("./local_files/users.txt");
    
    auto result = validate_input(argc, argv);

    int occurences{};
    std::string users = user_stats.load_users();
    auto logins = std::ranges::views::split(users, delimiter);
    if (result)
    {   
        // Case delete is set starts from here.

        auto [name, option] = *result;
        if (name == bread_key && option == delete_cmd)
        {
            user_stats.write_file(std::string(clear_file));
            std::println("Successfully deleted all users.");
            return 0;
        }

        for (const auto& delimited_part : logins) 
        {
            std::string uname = std::string(delimited_part.begin(), delimited_part.end()); 
            if (uname == name) {occurences += 1;}
        }

        if (occurences == 0) 
        {
            std::println("Nothing to delete, user not found."); 
            return 0;
        }

        std::string new_users;
        for (const auto& delimited_part : logins) 
        {
            std::string uname = std::string(delimited_part.begin(), delimited_part.end()); 
            if (uname != name) {new_users += uname + std::string(delimiter);}
        }

        user_stats.write_file(new_users);
        std::println("Successfully deleted user.");
        return 0;
    }

    // Case when delete not set starts from here.

    std::string name = std::string(argv[1]);
    for (const auto& delimited_part : logins) 
    {
        std::string uname = std::string(delimited_part.begin(), delimited_part.end()); 
        if (uname == name) {occurences += 1;}
    }

    if (occurences == 0)
    {
        std::println("Welcome, {}!", name);
        user_stats.save_user(name);
        return 0;
    }

    std::println("Hello again(x{}), {}", occurences + 1, name);
    user_stats.save_user(name);

    return 0;
}