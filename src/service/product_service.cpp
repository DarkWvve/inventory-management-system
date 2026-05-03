#include "include/product_service.h"


ProductService::ProductService(ProductRepository& pr) : pr(pr) {}

bool ProductService::createProduct(const Product& product) const
{
  return pr.createProduct(product);
}

std::vector<Product> ProductService::getAllProducts() const
{
  return pr.getAllProducts();
}

std::optional<Product> ProductService::findById(int id) const
{
  return pr.findById(id);
}

bool ProductService::updateProduct(const Product& product) const
{
  return pr.updateProduct(product);
}

bool ProductService::deleteProduct(int id) const
{
  return pr.deleteProduct(id);
}
