#pragma once
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
    sqlite3* getConnection();
};