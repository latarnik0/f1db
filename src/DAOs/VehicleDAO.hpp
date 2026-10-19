#pragma once
#include "QueryHandler/QueryHandler.hpp"

class VehicleDAO
{
private:
    sqlite3* db;

public:
    VehicleDAO(sqlite3* db);
    ~VehicleDAO();
};