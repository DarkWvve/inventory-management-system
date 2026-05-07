cmake_minimum_required(VERSION 3.20)

project(inventory_management_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

add_executable(inventory_management_system
    src/main.cpp

    src/db/database.cpp

    src/controller/user_controller.cpp
    src/controller/product_controller.cpp
    src/controller/stock_movement_controller.cpp
    src/controller/inventory_controller.cpp

    src/service/user_service.cpp
    src/service/product_service.cpp
    src/service/stock_movement_service.cpp
    src/service/inventory_service.cpp

    src/repositories/user_repository.cpp
    src/repositories/product_repository.cpp
    src/repositories/stock_movement_repository.cpp
    src/repositories/inventory_repository.cpp

    src/ui/console_utils.cpp
    )

target_include_directories(inventory_management_system PRIVATE
    ${SQLite3_INCLUDE_DIRS}
)

target_link_libraries(inventory_management_system PRIVATE
    ${SQLite3_LIBRARIES}
)
