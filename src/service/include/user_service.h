#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "../../repositories/include/user_repository.h"
#include "../../models/auth_user.h"

#include <optional>
#include <string>
#include <vector>


class UserService
{
private:
  UserRepository& ur;
  std::optional<AuthUser> user;

public:
  explicit UserService(UserRepository& ur);

  std::optional<AuthUser> login(const std::string& login, const std::string& password);

  bool isAuthorized() const;
  bool isAdmin() const;
  bool isManager() const;
  bool isWorker() const;

  std::optional<AuthUser> getCurrentUser() const;

  bool createUser(const User& user) const;
  std::vector<User> getAllUsers() const;
  bool changeUserRole(int id, int role_id) const;
  bool deleteUser(int id) const;
};


#endif
