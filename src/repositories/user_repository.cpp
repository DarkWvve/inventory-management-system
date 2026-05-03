#include "include/user_repository.h"


UserRepository::UserRepository(Database &db) :db(db) {}

std::optional<User> UserRepository::findByLogin(const std::string& login) const
{
  const char* sql = "SELECT id, login, password_hash, full_name, role_id, is_active FROM users WHERE login = ? LIMIT 1;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if (rc!=SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  rc = sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_TRANSIENT);
  if (rc!=SQLITE_OK)
  {
    std::cerr << "Failed to bind statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  std::optional<User> res = std::nullopt;

  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    User user;

    user.id = sqlite3_column_int(stmt, 0);
    user.login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    user.password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    user.full_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    user.role_id = sqlite3_column_int(stmt, 4);
    user.is_active = sqlite3_column_int(stmt, 5) != 0;

    res = user;
  }

  sqlite3_finalize(stmt);
  return res;
}

std::optional<std::string> UserRepository::findPasswordByLogin(const std::string& login) const
{
  const char* sql = "SELECT password_hash FROM users WHERE login = ? LIMIT 1;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if (rc!=SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  rc = sqlite3_bind_text(stmt, 1, login.c_str(), -1, SQLITE_TRANSIENT);
  if (rc!=SQLITE_OK)
  {
    std::cerr << "Failed to bind statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }


  std::optional<std::string> res = std::nullopt;

  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    res = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
  }

  sqlite3_finalize(stmt);
  return res;
}

bool UserRepository::createUser(const User& user) const
{
  const char* sql = "INSERT INTO users(login, password_hash, full_name, role_id, is_active) VALUES (?, ?, ?, ?, ?);";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_bind_text(stmt, 1, user.login.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, user.password_hash.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, user.full_name.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 4, user.role_id);
  sqlite3_bind_int(stmt, 5, user.is_active ? 1 : 0);

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to create user: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

std::vector<User> UserRepository::getAllUsers() const
{
  const char* sql = "SELECT id, login, password_hash, full_name, role_id, is_active FROM users ORDER BY id;";

  sqlite3_stmt* stmt = nullptr;
  std::vector<User> users;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return users;
  }

  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    User user;

    user.id = sqlite3_column_int(stmt, 0);
    user.login = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    user.password_hash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    user.full_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    user.role_id = sqlite3_column_int(stmt, 4);
    user.is_active = sqlite3_column_int(stmt, 5) != 0;

    users.push_back(user);
  }

  sqlite3_finalize(stmt);
  return users;
}

bool UserRepository::changeUserRole(int id, int role_id) const
{
  const char* sql = "UPDATE users SET role_id = ? WHERE id = ?;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_bind_int(stmt, 1, role_id);
  sqlite3_bind_int(stmt, 2, id);

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to change user role: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

bool UserRepository::deleteUser(int id) const
{
  const char* sql = "UPDATE users SET is_active = 0 WHERE id = ?;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_bind_int(stmt, 1, id);

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to delete user: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}
