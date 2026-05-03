#include "include/console_utils.h"

#include <cstdlib>
#include <iostream>
#include <limits>

namespace ConsoleUtils
{
  void ClearScreen()
  {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
  }

  void WaitEnter()
  {
    std::cout << "\nPress Enter to continue...";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
  }
}
