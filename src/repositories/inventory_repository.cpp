#include "include/inventory_repository.h"


InventoryRepository::InventoryRepository(Database& db) : db(db) {}

int InventoryRepository::createCheck(int user_id, const std::string& comment) const
{
  const char* sql =
    "INSERT INTO inventory_checks(check_date, user_id, status, comment)"
    " VALUES (datetime('now'), ?, 'OPEN', ?);";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return -1;
  }

  sqlite3_bind_int(stmt, 1, user_id);

  if(!comment.empty())
  {
    sqlite3_bind_text(stmt, 2, comment.c_str(), -1, SQLITE_TRANSIENT);
  }
  else
  {
    sqlite3_bind_null(stmt, 2);
  }

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to create inventory check: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return -1;
  }

  const int id = static_cast<int>(sqlite3_last_insert_rowid(db.get()));
  sqlite3_finalize(stmt);
  return id;
}

bool InventoryRepository::addCheckItem(const InventoryCheckItem& item) const
{
  const char* sql =
    "INSERT INTO inventory_check_items(inventory_check_id, product_id, system_quantity, actual_quantity)"
    " VALUES (?, ?, ?, ?);";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_bind_int(stmt, 1, item.inventory_check_id);
  sqlite3_bind_int(stmt, 2, item.product_id);
  sqlite3_bind_int(stmt, 3, item.system_quantity);
  sqlite3_bind_int(stmt, 4, item.actual_quantity);

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to add check item: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

bool InventoryRepository::closeCheck(int check_id) const
{
  const char* sql = "UPDATE inventory_checks SET status = 'CLOSED' WHERE id = ?;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_bind_int(stmt, 1, check_id);

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to close check: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

bool InventoryRepository::cancelCheck(int check_id) const
{
  const char* sql = "UPDATE inventory_checks SET status = 'CANCELLED' WHERE id = ?;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_bind_int(stmt, 1, check_id);

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to cancel check: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

std::vector<InventoryCheck> InventoryRepository::getAllChecks() const
{
  const char* sql =
    "SELECT id, check_date, user_id, status, comment"
    " FROM inventory_checks ORDER BY id DESC;";

  sqlite3_stmt* stmt = nullptr;
  std::vector<InventoryCheck> checks;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return checks;
  }

  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    InventoryCheck c;

    c.id = sqlite3_column_int(stmt, 0);
    c.check_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    c.user_id = sqlite3_column_int(stmt, 2);

    const std::string status_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    if(status_str == "OPEN")        c.status = InventoryCheckStatus::Open;
    else if(status_str == "CLOSED") c.status = InventoryCheckStatus::Closed;
    else                            c.status = InventoryCheckStatus::Cancelled;

    if(sqlite3_column_type(stmt, 4) != SQLITE_NULL)
    {
      c.comment = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    }

    checks.push_back(c);
  }

  sqlite3_finalize(stmt);
  return checks;
}

std::optional<InventoryCheck> InventoryRepository::findCheckById(int id) const
{
  const char* sql =
    "SELECT id, check_date, user_id, status, comment"
    " FROM inventory_checks WHERE id = ? LIMIT 1;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  sqlite3_bind_int(stmt, 1, id);

  std::optional<InventoryCheck> res = std::nullopt;

  if(sqlite3_step(stmt) == SQLITE_ROW)
  {
    InventoryCheck c;

    c.id = sqlite3_column_int(stmt, 0);
    c.check_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    c.user_id = sqlite3_column_int(stmt, 2);

    const std::string status_str = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    if(status_str == "OPEN")        c.status = InventoryCheckStatus::Open;
    else if(status_str == "CLOSED") c.status = InventoryCheckStatus::Closed;
    else                            c.status = InventoryCheckStatus::Cancelled;

    if(sqlite3_column_type(stmt, 4) != SQLITE_NULL)
    {
      c.comment = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    }

    res = c;
  }

  sqlite3_finalize(stmt);
  return res;
}

std::vector<InventoryCheckItem> InventoryRepository::getCheckItems(int check_id) const
{
  const char* sql =
    "SELECT id, inventory_check_id, product_id, system_quantity, actual_quantity, difference"
    " FROM inventory_check_items WHERE inventory_check_id = ?;";

  sqlite3_stmt* stmt = nullptr;
  std::vector<InventoryCheckItem> items;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return items;
  }

  sqlite3_bind_int(stmt, 1, check_id);

  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    InventoryCheckItem item;

    item.id = sqlite3_column_int(stmt, 0);
    item.inventory_check_id = sqlite3_column_int(stmt, 1);
    item.product_id = sqlite3_column_int(stmt, 2);
    item.system_quantity = sqlite3_column_int(stmt, 3);
    item.actual_quantity = sqlite3_column_int(stmt, 4);
    item.difference = sqlite3_column_int(stmt, 5);

    items.push_back(item);
  }

  sqlite3_finalize(stmt);
  return items;
}
