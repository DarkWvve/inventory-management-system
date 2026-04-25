#ifndef STOCK_MOVEMENT_H
#define STOCK_MOVEMENT_H

#include <string>
#include <optional>

enum class StockMovementType
{
  In,
  Out
};

struct StockMovement
{
  int id{};
  int product_id{};
  StockMovementType movement_type{};
  int quantity{};
  std::string movement_date;
  std::optional<int> supplier_id;
  int user_id{};
  std::optional<std::string> comment;
};

#endif