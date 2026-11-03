#pragma once
#include "Logger/Logger.hpp"
#include <sqlite3.h>
#include <string>

class DatabaseManager
{
private:
    sqlite3* db;

    void migrateSchema();
    bool tableHasColumn(const std::string& tableName, const std::string& columnName);
    void addColumnIfMissing(const std::string& tableName, const std::string& columnName, const std::string& columnDef);

public:
    DatabaseManager(const char* db_path);
    ~DatabaseManager();
    sqlite3* getConnection();
    bool isConnected() const;
};
