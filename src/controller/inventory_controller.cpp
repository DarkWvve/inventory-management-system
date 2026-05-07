#include "../ui/include/console_utils.h"
#include "include/inventory_controller.h"


InventoryController::InventoryController(InventoryService& is) : is(is) {}

void InventoryController::clearInput() const
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void InventoryController::CreateCheck(int user_id)
{
  std::string comment;

  clearInput();
  std::cout << "Comment (optional, press Enter to skip): ";
  std::getline(std::cin, comment);

  const int id = is.createCheck(user_id, comment);
  if(id < 0)
  {
    std::cerr << "Inventory check was not created.\n";
    return;
  }

  std::cout << "Inventory check created. ID: " << id << '\n';
}

void InventoryController::AddItem()
{
  int check_id = 0;
  int product_id = 0;
  int actual_quantity = 0;

  std::cout << "Check id: ";
  if(!(std::cin >> check_id))
  {
    clearInput();
    std::cerr << "Invalid check id.\n";
    return;
  }

  std::cout << "Product id: ";
  if(!(std::cin >> product_id))
  {
    clearInput();
    std::cerr << "Invalid product id.\n";
    return;
  }

  std::cout << "Actual quantity: ";
  if(!(std::cin >> actual_quantity))
  {
    clearInput();
    std::cerr << "Invalid quantity.\n";
    return;
  }

  if(!is.addCheckItem(check_id, product_id, actual_quantity))
  {
    std::cerr << "Item was not added.\n";
    return;
  }

  std::cout << "Item added.\n";
}

void InventoryController::CloseCheck()
{
  int id = 0;

  std::cout << "Check id: ";
  if(!(std::cin >> id))
  {
    clearInput();
    std::cerr << "Invalid id.\n";
    return;
  }

  if(!is.closeCheck(id))
  {
    std::cerr << "Check was not closed.\n";
    return;
  }

  std::cout << "Check closed.\n";
}

void InventoryController::CancelCheck()
{
  int id = 0;

  std::cout << "Check id: ";
  if(!(std::cin >> id))
  {
    clearInput();
    std::cerr << "Invalid id.\n";
    return;
  }

  if(!is.cancelCheck(id))
  {
    std::cerr << "Check was not cancelled.\n";
    return;
  }

  std::cout << "Check cancelled.\n";
}

void InventoryController::ShowAllChecks()
{
  auto checks = is.getAllChecks();

  if(checks.empty())
  {
    std::cout << "No inventory checks found.\n";
    return;
  }

  std::cout << "\n====== Inventory Checks ======\n";

  for(const auto& c : checks)
  {
    std::string status_str;
    if(c.status == InventoryCheckStatus::Open)           status_str = "OPEN";
    else if(c.status == InventoryCheckStatus::Closed)    status_str = "CLOSED";
    else                                                  status_str = "CANCELLED";

    std::cout << "ID: " << c.id << '\n';
    std::cout << "Date: " << c.check_date << '\n';
    std::cout << "User ID: " << c.user_id << '\n';
    std::cout << "Status: " << status_str << '\n';

    if(c.comment.has_value())
    {
      std::cout << "Comment: " << *c.comment << '\n';
    }

    std::cout << '\n';
  }
}

void InventoryController::ShowCheckItems()
{
  int check_id = 0;

  std::cout << "Check id: ";
  if(!(std::cin >> check_id))
  {
    clearInput();
    std::cerr << "Invalid id.\n";
    return;
  }

  auto items = is.getCheckItems(check_id);

  if(items.empty())
  {
    std::cout << "No items found for this check.\n";
    return;
  }

  std::cout << "\n====== Items of check " << check_id << " ======\n";

  for(const auto& item : items)
  {
    std::cout << "Product ID: " << item.product_id << '\n';
    std::cout << "System quantity: " << item.system_quantity << '\n';
    std::cout << "Actual quantity: " << item.actual_quantity << '\n';
    std::cout << "Difference: " << item.difference << "\n\n";
  }
}
