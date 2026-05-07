#ifndef INVENTORY_CONTROLLER_H
#define INVENTORY_CONTROLLER_H

#include "../../service/include/inventory_service.h"

#include <iostream>
#include <limits>

class InventoryController
{
private:
  InventoryService& is;

  void clearInput() const;

public:
  explicit InventoryController(InventoryService& is);

  void CreateCheck(int user_id);
  void AddItem();
  void CloseCheck();
  void CancelCheck();
  void ShowAllChecks();
  void ShowCheckItems();
};

#endif
