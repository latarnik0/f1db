cmake_minimum_required(VERSION 3.10)
project(f1db)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(f1db
    src/main.cpp
    
    # logic
    src/Logger/Logger.cpp
    src/DatabaseManager/DatabaseManager.cpp
    src/QueryHandler/QueryHandler.cpp
    
    # DAOs
    src/DAOs/InventoryDAO.cpp
    src/DAOs/PersonnelDAO.cpp
    src/DAOs/VehicleDAO.cpp
    src/DAOs/SessionDAO.cpp
    src/DAOs/TelemetryDAO.cpp
    
    src/Mocks/MockReadings.cpp
)
target_include_directories(f1db PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DatabaseManager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DAOs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QueryHandler
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Logger
)

target_link_libraries(f1db sqlite3)