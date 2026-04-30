#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H


#include "../../models/user.h"
#include "../../db/database.h"

#include <optional>


class UserRepository
{
private:
  Database &db;

public:
  explicit UserRepository(Database &db);

  //std::optional<User> createUser(User &user);

  std::optional<std::string> findByLogin(const char* name) const;
  std::optional<std::string> findPasswordByLogin(const char* name) const;
};

#endif
