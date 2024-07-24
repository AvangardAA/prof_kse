#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <ranges>
#include <optional>
#include <map>

using name_option = std::pair<std::string, std::string>;

constexpr std::string_view delimiter(",");
constexpr std::string_view delete_cmd("delete");
constexpr std::string_view bread_key("bread");
constexpr std::string_view clear_file("");

struct UserStatistics 
{
    UserStatistics(const std::string& fname) : filename(fname) {
        load_users();
    }

    std::string filename{};
    std::map<std::string, int> users_log{};

    auto map_to_log() -> std::string;

    auto load_users() -> void;
    auto save_user(const std::string& name) -> void;
    auto write_file(const std::string& content) -> void;
    auto count_logins(const std::string name) -> int;
};

auto UserStatistics::map_to_log() -> std::string
{
    std::stringstream ss;
    for (const auto& entry : users_log)
    {
        for (int i = 0; i < entry.second; i++)
        {
            ss << entry.first << std::string(delimiter);
        }
    }

    return ss.str();
}

auto UserStatistics::load_users() -> void
{
    std::string users{};
    std::ifstream file(filename);
    if (!file.is_open()) { return; }

    file >> users;
    file.close();

    std::stringstream ss(users);
    std::string name;
    while (std::getline(ss, name, ','))
    {
        users_log[name]++;
    }
    return;
}

auto UserStatistics::save_user(const std::string& name) -> void 
{
    users_log[name]++;
    write_file(map_to_log());
    return;
}

auto UserStatistics::write_file(const std::string& content) -> void
{
    std::ofstream file_o(filename);
    file_o << content;
    file_o.close();
    return;
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
    constexpr std::string_view fname("./local_files/users.txt");
    UserStatistics user_stats((std::string(fname)));

    auto result = validate_input(argc, argv);

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

        if (user_stats.users_log[name] == 0) 
        {
            std::println("Nothing to delete, user not found."); 
            return 0;
        }

        user_stats.users_log.erase(name);

        user_stats.write_file(user_stats.map_to_log());
        std::println("Successfully deleted user.");
        return 0;
    }

    // Case when delete not set starts from here.

    std::string name = std::string(argv[1]);
    int occurences = user_stats.users_log[name];
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