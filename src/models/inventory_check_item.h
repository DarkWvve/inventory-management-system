#ifndef INVENTORY_CHECK_ITEM_H
#define INVENTORY_CHECK_ITEM_H

struct InventoryCheckItem
{
  int id{};
  int inventory_check_id{};
  int product_id{};
  int system_quantity{};
  int actual_quantity{};
  int difference{};
};

#endif