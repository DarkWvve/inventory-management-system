#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <string>
#include <optional>

struct Supplier
{
  int id{};
  std::string name;
  std::optional<std::string> phone;
  std::optional<std::string> email;
  std::optional<std::string> address;
};

#endif