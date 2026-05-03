#ifndef USER_H
#define USER_H

#include <string>

struct User
{
  int id{};
  std::string login;
  std::string password_hash;
  std::string full_name;
  int role_id{};
  bool is_active{true};
};

#endif
