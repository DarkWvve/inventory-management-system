#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>


class Database
{
private:
  const char *path;

  sqlite3 *db = nullptr;

public:
  explicit Database(const char *path);
  ~Database();

  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  bool open();
  void close();

  [[nodiscard]]
  sqlite3 *get() const;
};


#endif
