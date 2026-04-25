#include "database.h"

#include <algorithm>
#include <iostream>
#include <ostream>

Database::Database(const char *path) : path(std::move(path)) {}
Database::~Database() { db_close(); }

bool Database::db_open ()
{
  if (db!=nullptr) return true;

  const int rc = sqlite3_open(path.c_str(), &db);

  if (rc != SQLITE_OK)
  {
    std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << '\n';
    db_close();
    return false;
  }

  return true;
}


void Database::db_close()
{
  if (db!=nullptr)
  {
    sqlite3_close(db);
    db = nullptr;
  }
}

sqlite3 *Database::get() const
{
  return db;
}
