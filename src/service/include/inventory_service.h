#ifndef INVENTORY_SERVICE_H
#define INVENTORY_SERVICE_H

#include "../../repositories/include/inventory_repository.h"
#include "../../repositories/include/stock_movement_repository.h"

#include <vector>
#include <optional>

class InventoryService
{
private:
  InventoryRepository& ir;
  StockMovementRepository& smr;

public:
  explicit InventoryService(InventoryRepository& ir, StockMovementRepository& smr);

  int createCheck(int user_id, const std::string& comment) const;
  bool addCheckItem(int check_id, int product_id, int actual_quantity) const;
  bool closeCheck(int check_id) const;
  bool cancelCheck(int check_id) const;

  std::vector<InventoryCheck> getAllChecks() const;
  std::optional<InventoryCheck> findCheckById(int id) const;
  std::vector<InventoryCheckItem> getCheckItems(int check_id) const;
};

#endif
