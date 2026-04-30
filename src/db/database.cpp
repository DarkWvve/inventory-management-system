#include "database.h"

#include <algorithm>
#include <iostream>


Database::Database(std::string path) : path(std::move(path)) {}
Database::~Database() { close(); }

bool Database::open ()
{
  if (db!=nullptr) return true;

  const int rc = sqlite3_open(path.c_str(), &db);

  if (rc != SQLITE_OK)
  {
    std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << '\n';
    close();
    return false;
  }else
  {
    puts("database opened!");
  }

  return true;
}


void Database::close()
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
