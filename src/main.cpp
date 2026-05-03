#include "db/database.h"
#include "repositories/include/user_repository.h"
#include "repositories/include/product_repository.h"
#include "service/include/user_service.h"
#include "service/include/product_service.h"
#include "controller/include/user_controller.h"
#include "controller/include/product_controller.h"
//#include <openssl/evp.h>
//#include <openssl/sha.h>


int main()
{
  Database db("./db/warehouse.db");

  if(!db.open())
  {
    return 1;
  }

  UserRepository ur(db);
  ProductRepository pr(db);

  UserService us(ur);
  ProductService ps(pr);

  ProductController pc(ps);
  UserController uc(us, pc);

  uc.Start();

  db.close();
  return 0;
} 
