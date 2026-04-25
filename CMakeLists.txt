cmake_minimum_required(VERSION 3.20)

project(inventory_management_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

add_executable(inventory_management_system
        src/main.cpp

        src/db/database.cpp
        src/db/database.h
)

target_link_libraries(inventory_management_system PRIVATE SQLite3::SQLite3)