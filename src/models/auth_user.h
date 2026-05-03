#ifndef AUTH_USER_H
#define AUTH_USER_H

#include "role.h"

#include <string>

struct AuthUser
{
  int id{};
  std::string login;
  Role role{};
};

#endif
