#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>

#include "Namespaces.hpp"

constexpr std::string_view users_file("./users/users.txt");
constexpr std::string_view writing_delimiter(",");
constexpr std::string_view newline("\n");

class UserUtils
{
public:
  UserUtils() = default;

  auto load_users() -> CustomTypes::login_encrypted;

  auto save_data(const CustomTypes::login_encrypted& users) -> void;
};