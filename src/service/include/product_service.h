#ifndef PRODUCT_SERVICE_H
#define PRODUCT_SERVICE_H

#include "../../repositories/include/product_repository.h"

#include <vector>
#include <optional>

class ProductService
{
private:
  ProductRepository& pr;

public:
  explicit ProductService(ProductRepository& pr);

  bool createProduct(const Product& product) const;
  std::vector<Product> getAllProducts() const;
  std::optional<Product> findById(int id) const;
  bool updateProduct(const Product& product) const;
  bool deleteProduct(int id) const;
};

#endif
