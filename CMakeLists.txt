cmake_minimum_required(VERSION 3.16)
project(f1db LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(f1db
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

# QML module — odkomentuj i uzupełnij, gdy pojawią się pliki .qml:
# qt_add_qml_module(f1db
#     URI F1DB
#     VERSION 1.0
#     QML_FILES
#         src/qml/Main.qml
# )

target_include_directories(f1db PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DatabaseManager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mocks
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DAOs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QueryHandler
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Logger
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
)

target_link_libraries(f1db PRIVATE
    Qt6::Quick
    sqlite3
)
