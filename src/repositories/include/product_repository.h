#ifndef PRODUCT_REPOSITORY_H
#define PRODUCT_REPOSITORY_H

#include "../../models/product.h"
#include "../../db/database.h"

#include <optional>
#include <vector>
#include <string>
#include <iostream>

class ProductRepository
{
private:
  Database& db;

public:
  explicit ProductRepository(Database& db);

  bool createProduct(const Product& product) const;
  std::vector<Product> getAllProducts() const;
  std::optional<Product> findById(int id) const;
  bool updateProduct(const Product& product) const;
  bool deleteProduct(int id) const;
};

#endif
