#include "UserUtils.h"

auto UserUtils::load_users() -> CustomTypes::login_encrypted
{
  std::ifstream file((std::string(users_file)));
  if (!file.is_open()) { throw; }

  CustomTypes::login_encrypted users_list{};

  std::string line{};
  while (std::getline(file, line)) 
  {
      std::stringstream ss(line);
      std::string name{};
      std::string encrypted{};
      if (std::getline(ss, name, ',') && std::getline(ss, encrypted)) 
      {
        users_list[name] = encrypted;
      }
  }

  file.close();

  return users_list;
}

auto UserUtils::save_data(const CustomTypes::login_encrypted& users) -> void
{
  std::ofstream file_o((std::string(users_file)));
  for (const auto& user : users)
  {
    file_o << user.first << std::string(writing_delimiter) << user.second << std::string(newline);
  }
  file_o.close();
}