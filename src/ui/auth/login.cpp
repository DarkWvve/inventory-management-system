#include "login.h"


int Authenticator::auth_user(const char* login, const char* password) const
{
  std::optional<std::string> l = ur.findByLogin(login);
  //std::optional<std::string> p = ur.findPasswordByLogin(login);

  // TODO сделать хеш пароль и сравнивать. пока так
  if (l == login)
  {

  }

  return 0;
}