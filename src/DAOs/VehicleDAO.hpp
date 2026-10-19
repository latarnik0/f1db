#pragma once
#include <sqlite3.h> 

class VehicleDAO
{
private:
    sqlite3* db;

public:
    VehicleDAO(sqlite3* db_connection);
    ~VehicleDAO();
};