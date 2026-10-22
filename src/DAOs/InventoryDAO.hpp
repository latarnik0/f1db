#pragma once
#include <sqlite3.h>
#include <string>

class InventoryDAO
{
private:
    sqlite3* db;

public:
    InventoryDAO(sqlite3* db_connection);
    ~InventoryDAO();

    int addPart(const std::string& partName, const std::string& partType);
};