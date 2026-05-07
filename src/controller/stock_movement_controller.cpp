#include "../ui/include/console_utils.h"
#include "include/stock_movement_controller.h"


StockMovementController::StockMovementController(StockMovementService& sms) : sms(sms) {}

void StockMovementController::clearInput() const
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void StockMovementController::AddIncoming(int user_id)
{
  int product_id = 0;
  int quantity = 0;
  std::string comment;

  std::cout << "Product id: ";
  if(!(std::cin >> product_id))
  {
    clearInput();
    std::cerr << "Invalid product id.\n";
    return;
  }

  std::cout << "Quantity: ";
  if(!(std::cin >> quantity))
  {
    clearInput();
    std::cerr << "Invalid quantity.\n";
    return;
  }

  clearInput();
  std::cout << "Comment (optional, press Enter to skip): ";
  std::getline(std::cin, comment);

  if(!sms.addIncoming(product_id, quantity, user_id, comment))
  {
    std::cerr << "Incoming movement was not created.\n";
    return;
  }

  std::cout << "Incoming movement created.\n";
}

void StockMovementController::AddOutgoing(int user_id)
{
  int product_id = 0;
  int quantity = 0;
  std::string comment;

  std::cout << "Product id: ";
  if(!(std::cin >> product_id))
  {
    clearInput();
    std::cerr << "Invalid product id.\n";
    return;
  }

  std::cout << "Quantity: ";
  if(!(std::cin >> quantity))
  {
    clearInput();
    std::cerr << "Invalid quantity.\n";
    return;
  }

  clearInput();
  std::cout << "Comment (optional, press Enter to skip): ";
  std::getline(std::cin, comment);

  if(!sms.addOutgoing(product_id, quantity, user_id, comment))
  {
    std::cerr << "Outgoing movement was not created.\n";
    return;
  }

  std::cout << "Outgoing movement created.\n";
}

void StockMovementController::ShowAllMovements()
{
  auto movements = sms.getAllMovements();

  if(movements.empty())
  {
    std::cout << "No movements found.\n";
    return;
  }

  std::cout << "\n====== Stock Movements ======\n";

  for(const auto& m : movements)
  {
    std::cout << "ID: " << m.id << '\n';
    std::cout << "Product ID: " << m.product_id << '\n';
    std::cout << "Type: " << (m.movement_type == StockMovementType::In ? "IN" : "OUT") << '\n';
    std::cout << "Quantity: " << m.quantity << '\n';
    std::cout << "Date: " << m.movement_date << '\n';
    std::cout << "User ID: " << m.user_id << '\n';

    if(m.comment.has_value())
    {
      std::cout << "Comment: " << *m.comment << '\n';
    }

    std::cout << '\n';
  }
}

void StockMovementController::ShowMovementsByProduct()
{
  int product_id = 0;

  std::cout << "Product id: ";
  if(!(std::cin >> product_id))
  {
    clearInput();
    std::cerr << "Invalid product id.\n";
    return;
  }

  auto movements = sms.getMovementsByProduct(product_id);

  if(movements.empty())
  {
    std::cout << "No movements found for this product.\n";
    return;
  }

  std::cout << "\n====== Movements for product " << product_id << " ======\n";

  for(const auto& m : movements)
  {
    std::cout << "ID: " << m.id << '\n';
    std::cout << "Type: " << (m.movement_type == StockMovementType::In ? "IN" : "OUT") << '\n';
    std::cout << "Quantity: " << m.quantity << '\n';
    std::cout << "Date: " << m.movement_date << '\n';

    if(m.comment.has_value())
    {
      std::cout << "Comment: " << *m.comment << '\n';
    }

    std::cout << '\n';
  }
}

void StockMovementController::ShowStockBalance()
{
  int product_id = 0;

  std::cout << "Product id: ";
  if(!(std::cin >> product_id))
  {
    clearInput();
    std::cerr << "Invalid product id.\n";
    return;
  }

  const int balance = sms.getStockBalance(product_id);
  std::cout << "Current stock balance for product " << product_id << ": " << balance << '\n';
}
