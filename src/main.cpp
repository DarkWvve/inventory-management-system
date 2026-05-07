#include "db/database.h"
#include "repositories/include/user_repository.h"
#include "repositories/include/product_repository.h"
#include "repositories/include/stock_movement_repository.h"
#include "repositories/include/inventory_repository.h"
#include "service/include/user_service.h"
#include "service/include/product_service.h"
#include "service/include/stock_movement_service.h"
#include "service/include/inventory_service.h"
#include "controller/include/user_controller.h"
#include "controller/include/product_controller.h"
#include "controller/include/stock_movement_controller.h"
#include "controller/include/inventory_controller.h"


int main()
{
  Database db("../db/warehouse.db");

  if(!db.open())
  {
    return 1;
  }

  UserRepository ur(db);
  ProductRepository pr(db);
  StockMovementRepository smr(db);
  InventoryRepository ir(db);

  UserService us(ur);
  ProductService ps(pr);
  StockMovementService sms(smr);
  InventoryService invs(ir, smr);

  ProductController pc(ps);
  StockMovementController smc(sms);
  InventoryController ic(invs);
  UserController uc(us, pc, smc, ic);

  uc.Start();

  db.close();
  return 0;
}
