#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "../../service/include/user_service.h"
#include "product_controller.h"

#include <stdint.h>
#include <iostream>
#include <optional>
#include <string>
#include <limits>

class UserController 
{
private:
  UserService& us;
  ProductController& pc;

  void clearInput() const;

public:
  explicit UserController(UserService& us, ProductController& pc);

  void Start();
  void ShowAdminPanel();
  void ShowManagerPanel();
  void ShowWorkerPanel();

  void CreateUser();
  void DeleteUser();
  void ChangeUserRole();
  void ShowUsers();

  std::optional<AuthUser> login(const char* login, const char* password);
  std::optional<AuthUser> LoginFromInput();
};


#endif
