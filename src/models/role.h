#ifndef ROLE_H
#define ROLE_H


#include <optional>


enum class Role
{
  ADMIN,
  MANAGER,
  WORKER
};


inline std::optional<Role> RoleFromId(const int id)
{
  switch(id)
  {
    case 1:
    return Role::ADMIN;

    case 2:
    return Role::MANAGER;
    
    case 3:
    return Role::WORKER;
    
    default:
    return std::nullopt;
  }
}


#endif
