#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

struct Product
{
  int id{};
  std::string name;
  std::string sku;
  int category_id{};
  std::string unit;
  int min_quantity{};
  bool is_active{true};
};

#endif