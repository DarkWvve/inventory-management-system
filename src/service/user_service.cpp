#include "include/user_service.h"

#include <iostream>


UserService::UserService(UserRepository& ur) : ur(ur) {}

std::optional<AuthUser> UserService::login(const std::string& login, const std::string& password)
{
  std::optional<User> _user = ur.findByLogin(login);

  if(!_user.has_value())
  {
    std::cerr << "Incorrect password or login\n";
    return std::nullopt;
  }

  if(_user->password_hash != password)
  {
    std::cerr << "Incorrect password or login\n";
    return std::nullopt;
  }

  if(!_user->is_active)
  {
    std::cerr << "User is not active\n";
    return std::nullopt;
  }

  AuthUser authUser;
  authUser.id = _user->id;
  authUser.login = std::string(_user->login);

  auto role = RoleFromId(_user->role_id);

  if(!role.has_value()) return std::nullopt;

  authUser.role = *role;
  user = authUser;

  return authUser;
}

bool UserService::isAuthorized() const
{
  return user.has_value();
}

bool UserService::isAdmin() const
{
  return user.has_value() && user->role == Role::ADMIN;
}

bool UserService::isManager() const
{
  return user.has_value() && user->role == Role::MANAGER;
}

bool UserService::isWorker() const
{
  return user.has_value() && user->role == Role::WORKER;
}

std::optional<AuthUser> UserService::getCurrentUser() const
{
  return user;
}

bool UserService::createUser(const User& user) const
{
  return ur.createUser(user);
}

std::vector<User> UserService::getAllUsers() const
{
  return ur.getAllUsers();
}

bool UserService::changeUserRole(int id, int role_id) const
{
  if(role_id < 1 || role_id > 3)
  {
    std::cerr << "Invalid role id.\n";
    return false;
  }

  return ur.changeUserRole(id, role_id);
}

bool UserService::deleteUser(int id) const
{
  return ur.deleteUser(id);
}
