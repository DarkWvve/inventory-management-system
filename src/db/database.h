#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

class Database
{
private:
  std::string path;

  sqlite3 *db = nullptr;

public:
  explicit Database(std::string path);
  ~Database();

  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  bool open();
  void close();


  [[nodiscard]]
  sqlite3 *get() const;
};


#endif
