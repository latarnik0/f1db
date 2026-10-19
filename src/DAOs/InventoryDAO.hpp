#pragma once
#include "QueryHandler/QueryHandler.hpp"

class InventoryDAO
{
private:
    sqlite3* db;

public:
    InventoryDAO(sqlite3* db);
    ~InventoryDAO();
};