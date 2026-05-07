#ifndef INVENTORY_REPOSITORY_H
#define INVENTORY_REPOSITORY_H

#include "../../models/inventory_check.h"
#include "../../models/inventory_check_item.h"
#include "../../db/database.h"

#include <vector>
#include <optional>
#include <iostream>

class InventoryRepository
{
private:
  Database& db;

public:
  explicit InventoryRepository(Database& db);

  int createCheck(int user_id, const std::string& comment) const;
  bool addCheckItem(const InventoryCheckItem& item) const;
  bool closeCheck(int check_id) const;
  bool cancelCheck(int check_id) const;

  std::vector<InventoryCheck> getAllChecks() const;
  std::optional<InventoryCheck> findCheckById(int id) const;
  std::vector<InventoryCheckItem> getCheckItems(int check_id) const;
};

#endif
