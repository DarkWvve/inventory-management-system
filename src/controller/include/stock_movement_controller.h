#ifndef STOCK_MOVEMENT_CONTROLLER_H
#define STOCK_MOVEMENT_CONTROLLER_H

#include "../../service/include/stock_movement_service.h"

#include <iostream>
#include <limits>

class StockMovementController
{
private:
  StockMovementService& sms;

  void clearInput() const;

public:
  explicit StockMovementController(StockMovementService& sms);

  void AddIncoming(int user_id);
  void AddOutgoing(int user_id);
  void ShowAllMovements();
  void ShowMovementsByProduct();
  void ShowStockBalance();
};

#endif
