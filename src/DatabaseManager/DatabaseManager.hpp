#pragma once
#include "Mocks/sensor_readings.hpp"
#include "Logger/Logger.hpp"
#include <iostream>
#include <sqlite3.h>

class DatabaseManager
{
private:
    sqlite3* db;

public:
    DatabaseManager(const char* db_path);
    ~DatabaseManager();
    sqlite3* getConnection(sqlite3* db);
    void insertMockReadings();
};