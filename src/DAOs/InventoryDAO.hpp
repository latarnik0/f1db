#pragma once
#include <sqlite3.h> 

class InventoryDAO
{
private:
    sqlite3* db;

public:
    InventoryDAO(sqlite3* db_connection);
    ~InventoryDAO();
};