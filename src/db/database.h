#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>


class Database
{
private:
  const char *path = "./db/warehouse.db";

  sqlite3 *db = nullptr;

public:
  explicit Database(const char *path);
  ~Database();

  const Database &operator=(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  bool db_open();
  void db_close();

  sqlite3 *get() const;
};


#endif
