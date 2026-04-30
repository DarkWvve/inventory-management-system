#ifndef LOGIN_H_
#define LOGIN_H_

#include "../../repositories/include/user_repository.h"


class Authenticator
{
public:
  UserRepository& ur;

private:
  explicit Authenticator(UserRepository& ur);

  int auth_user(sqlite3* db, std::string &login, std::string &password);

};

#endif
