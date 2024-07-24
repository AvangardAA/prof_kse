#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>

#include "Namespaces.hpp"

constexpr std::string_view users_file("./users/users.txt");

class UserUtils
{
public:
  UserUtils() = default;

  auto load_users() -> CustomTypes::login_encrypted;
};