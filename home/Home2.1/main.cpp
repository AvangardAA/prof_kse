
#define CPP2_IMPORT_STD          Yes

//=== Cpp2 type declarations ====================================================


#include "cpp2util.h"

#line 1 "main.cpp2"


//=== Cpp2 type definitions and function declarations ===========================

#line 1 "main.cpp2"
auto write_file(cpp2::impl::in<std::string> content) -> void;

#line 7 "main.cpp2"
[[nodiscard]] auto save_user(cpp2::impl::in<std::string> name) -> int;

#line 25 "main.cpp2"
[[nodiscard]] auto load_users() -> std::string;

#line 36 "main.cpp2"
[[nodiscard]] auto main(int const argc_, char** argv_) -> int;

//=== Cpp2 function definitions =================================================

#line 1 "main.cpp2"
auto write_file(cpp2::impl::in<std::string> content) -> void{
#line 2 "main.cpp2"
    std::ofstream file_o {"./local_files/users.txt"}; 
    file_o << content;
    CPP2_UFCS(close)(cpp2::move(file_o));
}

#line 7 "main.cpp2"
[[nodiscard]] auto save_user(cpp2::impl::in<std::string> name) -> int{
    std::ifstream file {"./local_files/users.txt"}; 
    if ((!(CPP2_UFCS(is_open)(file)))) 
    {
        write_file(name + ",");
        return 0; 
    }

    std::string users_line {}; 
    std::getline(file, users_line);
    CPP2_UFCS(close)(cpp2::move(file));

    users_line += name + ",";
    write_file(cpp2::move(users_line));

    return 0; 
}

#line 25 "main.cpp2"
[[nodiscard]] auto load_users() -> std::string{
    std::string users {}; 
    std::ifstream file {"./local_files/users.txt"}; 
    if ((!(CPP2_UFCS(is_open)(file)))) {return users; }

    file >> users;
    CPP2_UFCS(close)(cpp2::move(file));

    return users; 
}

#line 36 "main.cpp2"
[[nodiscard]] auto main(int const argc_, char** argv_) -> int{
    auto const args = cpp2::make_args(argc_, argv_); 
#line 37 "main.cpp2"
    if ((cpp2::impl::cmp_less(args.argc,2) || cpp2::impl::cmp_greater(args.argc,3))) 
    {
        std::cout << "Unsupported number of args" << std::endl;
        return 1; 
    }

    std::string option {}; 
    if ((args.argc == 3)) 
    {
        option = std::string(CPP2_ASSERT_IN_BOUNDS_LITERAL(args.argv, 2));
        if ((option != "delete")) 
        {
            std::cout << "Unsupported second argument. Support only: 'delete'." << std::endl;
            return 1; 
        }
    }

    auto users {load_users()}; 
    std::string name {CPP2_ASSERT_IN_BOUNDS_LITERAL(args.argv, 1)}; 

    if ((name == "bread" && option != "")) 
    {
        write_file("");
        std::cout << "Successfully deleted all users." << std::endl;
        return 0; 
    }

    std::string_view delimiter {","}; 
    auto logins {std::ranges::views::split(cpp2::move(users), cpp2::move(delimiter))}; 
    int occurences {0}; 

    for ( auto const& delimited_part : logins ) 
    {
        auto uname {std::string(CPP2_UFCS(begin)(delimited_part), CPP2_UFCS(end)(delimited_part))}; 
        if ((cpp2::move(uname) == name)) {occurences += 1; }
    }

    if ((occurences == 0 && option != "")) 
    {
        std::cout << "Nothing to delete, user not found." << std::endl;
        return 0; 
    }

    if ((occurences == 0)) 
    {
        std::cout << "Welcome, " << name << "!" << std::endl;
        int auto_1 {save_user(name)}; 
        return 0; 
    }

    if ((cpp2::move(option) != "")) 
    {
        std::string new_users {}; 
        for ( auto const& delimited_part : cpp2::move(logins) ) 
        {
            auto uname {std::string(CPP2_UFCS(begin)(delimited_part), CPP2_UFCS(end)(delimited_part))}; 
            if ((uname != name)) {new_users += cpp2::move(uname) + ","; }
        }

        write_file(cpp2::move(new_users));
        std::cout << "Successfully deleted user." << std::endl;
        return 0; 
    }

    std::cout << "Hello again(x" << cpp2::move(occurences) + 1 << "), " << name << std::endl;
    int auto_2 {save_user(cpp2::move(name))}; 

    return 0; 
}

