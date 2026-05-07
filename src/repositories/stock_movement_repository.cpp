#include "include/stock_movement_repository.h"


StockMovementRepository::StockMovementRepository(Database& db) : db(db) {}

bool StockMovementRepository::createMovement(const StockMovement& movement) const
{
  const char* sql =
    "INSERT INTO stock_movements(product_id, movement_type, quantity, movement_date, user_id, comment)"
    " VALUES (?, ?, ?, datetime('now'), ?, ?);";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  const std::string type_str = (movement.movement_type == StockMovementType::In) ? "IN" : "OUT";

  sqlite3_bind_int(stmt, 1, movement.product_id);
  sqlite3_bind_text(stmt, 2, type_str.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 3, movement.quantity);
  sqlite3_bind_int(stmt, 4, movement.user_id);

  if(movement.comment.has_value())
  {
    sqlite3_bind_text(stmt, 5, movement.comment->c_str(), -1, SQLITE_TRANSIENT);
  }
  else
  {
    sqlite3_bind_null(stmt, 5);
  }

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to create movement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

std::vector<StockMovement> StockMovementRepository::getAllMovements() const
{
  const char* sql =
    "SELECT id, product_id, movement_type, quantity, movement_date, user_id, comment"
    " FROM stock_movements ORDER BY id DESC;";

  sqlite3_stmt* stmt = nullptr;
  std::vector<StockMovement> movements;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return movements;
  }

  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    StockMovement m;

    m.id = sqlite3_column_int(stmt, 0);
    m.product_id = sqlite3_column_int(stmt, 1);

    const std::string type_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    m.movement_type = (type_str == "IN") ? StockMovementType::In : StockMovementType::Out;

    m.quantity = sqlite3_column_int(stmt, 3);
    m.movement_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    m.user_id = sqlite3_column_int(stmt, 5);

    if(sqlite3_column_type(stmt, 6) != SQLITE_NULL)
    {
      m.comment = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    }

    movements.push_back(m);
  }

  sqlite3_finalize(stmt);
  return movements;
}

std::vector<StockMovement> StockMovementRepository::getMovementsByProduct(int product_id) const
{
  const char* sql =
    "SELECT id, product_id, movement_type, quantity, movement_date, user_id, comment"
    " FROM stock_movements WHERE product_id = ? ORDER BY id DESC;";

  sqlite3_stmt* stmt = nullptr;
  std::vector<StockMovement> movements;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return movements;
  }

  sqlite3_bind_int(stmt, 1, product_id);

  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    StockMovement m;

    m.id = sqlite3_column_int(stmt, 0);
    m.product_id = sqlite3_column_int(stmt, 1);

    const std::string type_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    m.movement_type = (type_str == "IN") ? StockMovementType::In : StockMovementType::Out;

    m.quantity = sqlite3_column_int(stmt, 3);
    m.movement_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    m.user_id = sqlite3_column_int(stmt, 5);

    if(sqlite3_column_type(stmt, 6) != SQLITE_NULL)
    {
      m.comment = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    }

    movements.push_back(m);
  }

  sqlite3_finalize(stmt);
  return movements;
}

int StockMovementRepository::getStockBalance(int product_id) const
{
  const char* sql =
    "SELECT"
    " COALESCE(SUM(CASE WHEN movement_type='IN' THEN quantity ELSE -quantity END), 0)"
    " FROM stock_movements WHERE product_id = ?;";

  sqlite3_stmt* stmt = nullptr;
  int balance = 0;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return balance;
  }

  sqlite3_bind_int(stmt, 1, product_id);

  if(sqlite3_step(stmt) == SQLITE_ROW)
  {
    balance = sqlite3_column_int(stmt, 0);
  }

  sqlite3_finalize(stmt);
  return balance;
}
