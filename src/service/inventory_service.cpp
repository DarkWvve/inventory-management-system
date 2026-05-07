#include "include/inventory_service.h"


InventoryService::InventoryService(InventoryRepository& ir, StockMovementRepository& smr)
  : ir(ir), smr(smr) {}

int InventoryService::createCheck(int user_id, const std::string& comment) const
{
  return ir.createCheck(user_id, comment);
}

bool InventoryService::addCheckItem(int check_id, int product_id, int actual_quantity) const
{
  const int system_quantity = smr.getStockBalance(product_id);

  InventoryCheckItem item;
  item.inventory_check_id = check_id;
  item.product_id = product_id;
  item.system_quantity = system_quantity;
  item.actual_quantity = actual_quantity;

  return ir.addCheckItem(item);
}

bool InventoryService::closeCheck(int check_id) const
{
  return ir.closeCheck(check_id);
}

bool InventoryService::cancelCheck(int check_id) const
{
  return ir.cancelCheck(check_id);
}

std::vector<InventoryCheck> InventoryService::getAllChecks() const
{
  return ir.getAllChecks();
}

std::optional<InventoryCheck> InventoryService::findCheckById(int id) const
{
  return ir.findCheckById(id);
}

std::vector<InventoryCheckItem> InventoryService::getCheckItems(int check_id) const
{
  return ir.getCheckItems(check_id);
}
