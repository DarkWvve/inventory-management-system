#include "include/user_repository.h"

#include <iostream>
#include <ostream>


UserRepository::UserRepository(Database &db) :db(db) {}



std::optional<std::string> UserRepository::findByLogin(const char* name) const
{
  const char* sql = "SELECT name FROM users WHERE name = ? LIMIT 1;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if (rc!=SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errstr(rc) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  rc = sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
  if (rc!=SQLITE_OK)
  {
    std::cerr << "Failed to bind statement: " << sqlite3_errstr(rc) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  std::optional<std::string> res = std::nullopt;

  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    User user;

    user.login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

    res = user.login;

  }

  sqlite3_finalize(stmt);
  return res;
}

std::optional<std::string> UserRepository::findPasswordByLogin(const char* name) const
{
  const char* sql = "SELECT password_hash FROM users WHERE name = ? LIMIT 1;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if (rc!=SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errstr(rc) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  rc = sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
  if (rc!=SQLITE_OK)
  {
    std::cerr << "Failed to bind statement: " << sqlite3_errstr(rc) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }


  std::optional<std::string> res = std::nullopt;

  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    User user;

    user.password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

    res = user.password_hash;
  }

  sqlite3_finalize(stmt);
  return res;
}

