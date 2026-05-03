#ifndef PRODUCT_CONTROLLER_H
#define PRODUCT_CONTROLLER_H

#include "../../service/include/product_service.h"

#include <iostream>
#include <limits>

class ProductController
{
private:
  ProductService& ps;

  void clearInput() const;

public:
  explicit ProductController(ProductService& ps);

  void AddProduct();
  void EditProduct();
  void DeleteProduct();
  void ShowAllProducts();
};

#endif
