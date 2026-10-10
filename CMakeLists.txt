cmake_minimum_required(VERSION 3.10)
project(f1db)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(f1db src/main.cpp)
target_link_libraries(f1db sqlite3)