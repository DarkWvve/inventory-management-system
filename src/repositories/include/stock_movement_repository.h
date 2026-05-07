#ifndef STOCK_MOVEMENT_REPOSITORY_H
#define STOCK_MOVEMENT_REPOSITORY_H

#include "../../models/stock_movement.h"
#include "../../db/database.h"

#include <vector>
#include <iostream>

class StockMovementRepository
{
private:
  Database& db;

public:
  explicit StockMovementRepository(Database& db);

  bool createMovement(const StockMovement& movement) const;
  std::vector<StockMovement> getAllMovements() const;
  std::vector<StockMovement> getMovementsByProduct(int product_id) const;
  int getStockBalance(int product_id) const;
};

#endif
