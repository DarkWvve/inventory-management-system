#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H


#include "../../models/user.h"
#include "../../db/database.h"

#include <optional>
#include <iostream>
#include <string>
#include <vector>


class UserRepository
{
private:
  Database &db;

public:
  explicit UserRepository(Database &db);

  bool createUser(const User& user) const;
  std::vector<User> getAllUsers() const;
  bool changeUserRole(int id, int role_id) const;
  bool deleteUser(int id) const;

  std::optional<User> findByLogin(const std::string& login) const;
  std::optional<std::string> findPasswordByLogin(const std::string& login) const;
};

#endif
