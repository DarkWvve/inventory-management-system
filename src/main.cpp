#include <iostream>
#include "db/database.h"

int main()
{
  Database db("./db/warehouse.db");
  db.open();


  db.close();
  return 0;
}