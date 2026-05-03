#include "../ui/include/console_utils.h"
#include "include/user_controller.h"


UserController::UserController(UserService& us, ProductController& pc) : us(us), pc(pc) {}

void UserController::clearInput() const
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::optional<AuthUser>UserController::login(const char* login, const char* password)
{
  return us.login(login, password);
}

std::optional<AuthUser> UserController::LoginFromInput()
{
  ConsoleUtils::ClearScreen();

  std::string login;
  std::string password;

  std::cout << "Login: ";
  std::cin >> login;

  std::cout << "Password: ";
  std::cin >> password;

  return us.login(login, password);
}

void UserController::Start()
{
  auto authUser = LoginFromInput();

  if(!authUser.has_value())
  {
    std::cerr << "Incorrect login or password.\n";
    ConsoleUtils::WaitEnter();
    return;
  }

  if(authUser->role == Role::ADMIN)
  {
    ShowAdminPanel();
    return;
  }

  if(authUser->role == Role::MANAGER)
  {
    ShowManagerPanel();
    return;
  }

  if(authUser->role == Role::WORKER)
  {
    ShowWorkerPanel();
    return;
  }
}

void UserController::CreateUser()
{
  User user;

  std::cout << "Login: ";
  std::cin >> user.login;

  std::cout << "Password: ";
  std::cin >> user.password_hash;

  std::cout << "Full name: ";
  clearInput();
  std::getline(std::cin, user.full_name);

  std::cout << "Role id (1-admin, 2-manager, 3-worker): ";
  if(!(std::cin >> user.role_id))
  {
    clearInput();
    std::cerr << "Invalid role id.\n";
    return;
  }

  user.is_active = true;

  if(!us.createUser(user))
  {
    std::cerr << "User was not created.\n";
    return;
  }

  std::cout << "User created.\n";
}

void UserController::DeleteUser()
{
  int id = 0;

  std::cout << "User id: ";
  if(!(std::cin >> id))
  {
    clearInput();
    std::cerr << "Invalid id.\n";
    return;
  }

  if(!us.deleteUser(id))
  {
    std::cerr << "User was not deleted.\n";
    return;
  }

  std::cout << "User deleted.\n";
}

void UserController::ChangeUserRole()
{
  int id = 0;
  int role_id = 0;

  std::cout << "User id: ";
  if(!(std::cin >> id))
  {
    clearInput();
    std::cerr << "Invalid id.\n";
    return;
  }

  std::cout << "New role id (1-admin, 2-manager, 3-worker): ";
  if(!(std::cin >> role_id))
  {
    clearInput();
    std::cerr << "Invalid role id.\n";
    return;
  }

  if(!us.changeUserRole(id, role_id))
  {
    std::cerr << "User role was not changed.\n";
    return;
  }

  std::cout << "User role changed.\n";
}

void UserController::ShowUsers()
{
  auto users = us.getAllUsers();

  if(users.empty())
  {
    std::cout << "Users list is empty.\n";
    return;
  }

  std::cout << "\n====== Users ======\n";

  for(const auto& user : users)
  {
    std::cout << "ID: " << user.id << '\n';
    std::cout << "Login: " << user.login << '\n';
    std::cout << "Full name: " << user.full_name << '\n';
    std::cout << "Role id: " << user.role_id << '\n';
    std::cout << "Active: " << (user.is_active ? "yes" : "no") << "\n\n";
  }
}

void UserController::ShowAdminPanel()
{
  std::optional<AuthUser> user = us.getCurrentUser();

  if(!user.has_value() || 
      user->role != Role::ADMIN)
  {
    std::cerr << "U are not admin.\n";
    return;
  }

  auto PrintActions = [this, &user]()
  {
    std::cout << "\n====== Admin panel ======\n";
    std::cout << "Hello, " << user->login << "!\n";
    std::cout << "1. Create user\n";
    std::cout << "2. Delete user\n";
    std::cout << "3. Change user role\n";
    std::cout << "4. Show users\n";
    std::cout << "5. Add product\n";
    std::cout << "6. Edit product\n";
    std::cout << "7. Delete product\n";
    std::cout << "8. Show all products\n";
    std::cout << "0. Exit\n";
  };

  for(;;)
  {
    int choise = 0;
    
    ConsoleUtils::ClearScreen();
    PrintActions();

    if(!(std::cin >> choise))
    {
      clearInput();
      std::cerr << "Invalid input.\n";
      ConsoleUtils::WaitEnter();
      continue;
    }
    
    switch (choise) {
    case 0:
      return;

    case 1:
      CreateUser();
      ConsoleUtils::WaitEnter();
      break;

    case 2:
      DeleteUser();
      ConsoleUtils::WaitEnter();
      break;

    case 3:
      ChangeUserRole();
      ConsoleUtils::WaitEnter();
      break;

    case 4:
      ShowUsers();
      ConsoleUtils::WaitEnter();
      break;

    case 5:
      pc.AddProduct();
      ConsoleUtils::WaitEnter();
      break;

    case 6:
      pc.EditProduct();
      ConsoleUtils::WaitEnter();
      break;

    case 7:
      pc.DeleteProduct();
      ConsoleUtils::WaitEnter();
      break;

    case 8:
      pc.ShowAllProducts();
      ConsoleUtils::WaitEnter();
      break;

    default:
      std::cerr << "Unknown command.\n";
      ConsoleUtils::WaitEnter();
      break;
    }
  }
}

void UserController::ShowManagerPanel()
{
  if(!us.isManager())
  {
    std::cerr << "U are not manager.\n";
    return;
  }

  auto user = us.getCurrentUser();

  auto PrintActions = [this, &user]()
  {
    std::cout << "\n====== Manager panel ======\n";
    if(user.has_value()) std::cout << "Hello, " << user->login << "!\n";
    std::cout << "1. Add product\n";
    std::cout << "2. Edit product\n";
    std::cout << "3. Delete product\n";
    std::cout << "4. Show all products\n";
    std::cout << "0. Exit\n";
  };

  for(;;)
  {
    int choise = 0;

    ConsoleUtils::ClearScreen();
    PrintActions();

    if(!(std::cin >> choise))
    {
      clearInput();
      std::cerr << "Invalid input.\n";
      ConsoleUtils::WaitEnter();
      continue;
    }

    switch(choise)
    {
    case 0:
      return;

    case 1:
      pc.AddProduct();
      ConsoleUtils::WaitEnter();
      break;

    case 2:
      pc.EditProduct();
      ConsoleUtils::WaitEnter();
      break;

    case 3:
      pc.DeleteProduct();
      ConsoleUtils::WaitEnter();
      break;

    case 4:
      pc.ShowAllProducts();
      ConsoleUtils::WaitEnter();
      break;

    default:
      std::cerr << "Unknown command.\n";
      ConsoleUtils::WaitEnter();
      break;
    }
  }
}

void UserController::ShowWorkerPanel()
{
  if(!us.isWorker())
  {
    std::cerr << "U are not worker.\n";
    return;
  }

  auto user = us.getCurrentUser();

  auto PrintActions = [this, &user]()
  {
    std::cout << "\n====== Worker panel ======\n";
    if(user.has_value()) std::cout << "Hello, " << user->login << "!\n";
    std::cout << "1. Show all products\n";
    std::cout << "0. Exit\n";
  };

  for(;;)
  {
    int choise = 0;

    ConsoleUtils::ClearScreen();
    PrintActions();

    if(!(std::cin >> choise))
    {
      clearInput();
      std::cerr << "Invalid input.\n";
      ConsoleUtils::WaitEnter();
      continue;
    }

    switch(choise)
    {
    case 0:
      return;

    case 1:
      pc.ShowAllProducts();
      ConsoleUtils::WaitEnter();
      break;

    default:
      std::cerr << "Unknown command.\n";
      ConsoleUtils::WaitEnter();
      break;
    }
  }
}
