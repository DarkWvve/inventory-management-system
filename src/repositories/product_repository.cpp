#include "include/product_repository.h"


ProductRepository::ProductRepository(Database& db) : db(db) {}

bool ProductRepository::createProduct(const Product& product) const
{
  const char* sql = "INSERT INTO products(name, sku, category_id, unit, min_quantity, is_active) VALUES (?, ?, ?, ?, ?, ?);";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_bind_text(stmt, 1, product.name.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, product.sku.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 3, product.category_id);
  sqlite3_bind_text(stmt, 4, product.unit.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 5, product.min_quantity);
  sqlite3_bind_int(stmt, 6, product.is_active ? 1 : 0);

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to create product: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

std::vector<Product> ProductRepository::getAllProducts() const
{
  const char* sql = "SELECT id, name, sku, category_id, unit, min_quantity, is_active FROM products ORDER BY id;";

  sqlite3_stmt* stmt = nullptr;
  std::vector<Product> products;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return products;
  }

  while(sqlite3_step(stmt) == SQLITE_ROW)
  {
    Product product;

    product.id = sqlite3_column_int(stmt, 0);
    product.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    product.sku = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    product.category_id = sqlite3_column_int(stmt, 3);
    product.unit = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    product.min_quantity = sqlite3_column_int(stmt, 5);
    product.is_active = sqlite3_column_int(stmt, 6) != 0;

    products.push_back(product);
  }

  sqlite3_finalize(stmt);
  return products;
}

std::optional<Product> ProductRepository::findById(int id) const
{
  const char* sql = "SELECT id, name, sku, category_id, unit, min_quantity, is_active FROM products WHERE id = ? LIMIT 1;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return std::nullopt;
  }

  sqlite3_bind_int(stmt, 1, id);

  std::optional<Product> res = std::nullopt;

  if(sqlite3_step(stmt) == SQLITE_ROW)
  {
    Product product;

    product.id = sqlite3_column_int(stmt, 0);
    product.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    product.sku = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    product.category_id = sqlite3_column_int(stmt, 3);
    product.unit = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    product.min_quantity = sqlite3_column_int(stmt, 5);
    product.is_active = sqlite3_column_int(stmt, 6) != 0;

    res = product;
  }

  sqlite3_finalize(stmt);
  return res;
}

bool ProductRepository::updateProduct(const Product& product) const
{
  const char* sql = "UPDATE products SET name = ?, sku = ?, category_id = ?, unit = ?, min_quantity = ?, is_active = ? WHERE id = ?;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_bind_text(stmt, 1, product.name.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, product.sku.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 3, product.category_id);
  sqlite3_bind_text(stmt, 4, product.unit.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 5, product.min_quantity);
  sqlite3_bind_int(stmt, 6, product.is_active ? 1 : 0);
  sqlite3_bind_int(stmt, 7, product.id);

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to update product: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

bool ProductRepository::deleteProduct(int id) const
{
  const char* sql = "UPDATE products SET is_active = 0 WHERE id = ?;";

  sqlite3_stmt* stmt = nullptr;

  int rc = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if(rc != SQLITE_OK)
  {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_bind_int(stmt, 1, id);

  rc = sqlite3_step(stmt);
  if(rc != SQLITE_DONE)
  {
    std::cerr << "Failed to delete product: " << sqlite3_errmsg(db.get()) << '\n';
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}
