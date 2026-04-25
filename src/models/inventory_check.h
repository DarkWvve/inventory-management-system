#ifndef INVENTORY_CHECK_H
#define INVENTORY_CHECK_H

#include <string>
#include <optional>

enum class InventoryCheckStatus
{
  Open,
  Closed,
  Cancelled
};

struct InventoryCheck
{
  int id{};
  std::string check_date;
  int user_id{};
  InventoryCheckStatus status{InventoryCheckStatus::Open};
  std::optional<std::string> comment;
};

#endif