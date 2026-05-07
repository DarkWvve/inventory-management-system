#include "include/stock_movement_service.h"

#include <iostream>


StockMovementService::StockMovementService(StockMovementRepository& smr) : smr(smr) {}

bool StockMovementService::addIncoming(int product_id, int quantity, int user_id, const std::string& comment) const
{
  if(quantity <= 0)
  {
    std::cerr << "Quantity must be greater than zero.\n";
    return false;
  }

  StockMovement m;
  m.product_id = product_id;
  m.movement_type = StockMovementType::In;
  m.quantity = quantity;
  m.user_id = user_id;

  if(!comment.empty())
  {
    m.comment = comment;
  }

  return smr.createMovement(m);
}

bool StockMovementService::addOutgoing(int product_id, int quantity, int user_id, const std::string& comment) const
{
  if(quantity <= 0)
  {
    std::cerr << "Quantity must be greater than zero.\n";
    return false;
  }

  const int balance = smr.getStockBalance(product_id);
  if(balance < quantity)
  {
    std::cerr << "Not enough stock. Current balance: " << balance << '\n';
    return false;
  }

  StockMovement m;
  m.product_id = product_id;
  m.movement_type = StockMovementType::Out;
  m.quantity = quantity;
  m.user_id = user_id;

  if(!comment.empty())
  {
    m.comment = comment;
  }

  return smr.createMovement(m);
}

std::vector<StockMovement> StockMovementService::getAllMovements() const
{
  return smr.getAllMovements();
}

std::vector<StockMovement> StockMovementService::getMovementsByProduct(int product_id) const
{
  return smr.getMovementsByProduct(product_id);
}

int StockMovementService::getStockBalance(int product_id) const
{
  return smr.getStockBalance(product_id);
}
