cmake_minimum_required(VERSION 3.10)
project(f1db)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(f1db
    src/main.cpp
    src/DatabaseManager/DatabaseManager.cpp
    src/mocks/sensory_readings.cpp
)

target_include_directories(f1db PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DatabaseManager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mocks
)