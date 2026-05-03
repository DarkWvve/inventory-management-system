cmake_minimum_required(VERSION 3.20)

project(inventory_management_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(inventory_management_system
    src/main.cpp
    
    src/db/database.cpp
    
    src/controller/user_controller.cpp
    src/controller/product_controller.cpp

    src/service/user_service.cpp
    src/service/product_service.cpp

    src/repositories/user_repository.cpp
    src/repositories/product_repository.cpp

    src/ui/mainloop.cpp
    src/ui/console_utils.cpp
    src/ui/auth/session_view.cpp
    )

target_include_directories(inventory_management_system PRIVATE
    ${SQLite3_INCLUDE_DIRS}
)

target_link_libraries(inventory_management_system PRIVATE
    ${SQLite3_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
)
