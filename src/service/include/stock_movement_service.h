#ifndef STOCK_MOVEMENT_SERVICE_H
#define STOCK_MOVEMENT_SERVICE_H

#include "../../repositories/include/stock_movement_repository.h"

#include <vector>

class StockMovementService
{
private:
  StockMovementRepository& smr;

public:
  explicit StockMovementService(StockMovementRepository& smr);

  bool addIncoming(int product_id, int quantity, int user_id, const std::string& comment) const;
  bool addOutgoing(int product_id, int quantity, int user_id, const std::string& comment) const;
  std::vector<StockMovement> getAllMovements() const;
  std::vector<StockMovement> getMovementsByProduct(int product_id) const;
  int getStockBalance(int product_id) const;
};

#endif
