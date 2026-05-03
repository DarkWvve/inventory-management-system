#include "../ui/include/console_utils.h"
#include "include/product_controller.h"


ProductController::ProductController(ProductService& ps) : ps(ps) {}

void ProductController::clearInput() const
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ProductController::AddProduct()
{
  Product product;

  std::cout << "Product name: ";
  std::cin >> product.name;

  std::cout << "SKU: ";
  std::cin >> product.sku;

  std::cout << "Category id: ";
  if(!(std::cin >> product.category_id))
  {
    clearInput();
    std::cerr << "Invalid category id.\n";
    return;
  }

  std::cout << "Unit: ";
  std::cin >> product.unit;

  std::cout << "Min quantity: ";
  if(!(std::cin >> product.min_quantity))
  {
    clearInput();
    std::cerr << "Invalid quantity.\n";
    return;
  }

  product.is_active = true;

  if(!ps.createProduct(product))
  {
    std::cerr << "Product was not created.\n";
    return;
  }

  std::cout << "Product created.\n";
}

void ProductController::EditProduct()
{
  int id = 0;

  std::cout << "Product id: ";
  if(!(std::cin >> id))
  {
    clearInput();
    std::cerr << "Invalid id.\n";
    return;
  }

  auto product = ps.findById(id);
  if(!product.has_value())
  {
    std::cerr << "Product not found.\n";
    return;
  }

  std::cout << "New name: ";
  std::cin >> product->name;

  std::cout << "New SKU: ";
  std::cin >> product->sku;

  std::cout << "New category id: ";
  if(!(std::cin >> product->category_id))
  {
    clearInput();
    std::cerr << "Invalid category id.\n";
    return;
  }

  std::cout << "New unit: ";
  std::cin >> product->unit;

  std::cout << "New min quantity: ";
  if(!(std::cin >> product->min_quantity))
  {
    clearInput();
    std::cerr << "Invalid quantity.\n";
    return;
  }

  if(!ps.updateProduct(*product))
  {
    std::cerr << "Product was not updated.\n";
    return;
  }

  std::cout << "Product updated.\n";
}

void ProductController::DeleteProduct()
{
  int id = 0;

  std::cout << "Product id: ";
  if(!(std::cin >> id))
  {
    clearInput();
    std::cerr << "Invalid id.\n";
    return;
  }

  if(!ps.deleteProduct(id))
  {
    std::cerr << "Product was not deleted.\n";
    return;
  }

  std::cout << "Product deleted.\n";
}

void ProductController::ShowAllProducts()
{
  auto products = ps.getAllProducts();

  if(products.empty())
  {
    std::cout << "Products list is empty.\n";
    return;
  }

  std::cout << "\n====== Products ======\n";

  for(const auto& product : products)
  {
    std::cout << "ID: " << product.id << '\n';
    std::cout << "Name: " << product.name << '\n';
    std::cout << "SKU: " << product.sku << '\n';
    std::cout << "Category ID: " << product.category_id << '\n';
    std::cout << "Unit: " << product.unit << '\n';
    std::cout << "Min quantity: " << product.min_quantity << '\n';
    std::cout << "Active: " << (product.is_active ? "yes" : "no") << "\n\n";
  }
}
