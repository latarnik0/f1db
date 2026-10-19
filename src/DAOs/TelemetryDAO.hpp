#pragma once
#include <sqlite3.h> 

class TelemetryDAO
{
private:
    sqlite3* db;

public:
    TelemetryDAO(sqlite3* db_connection);
    ~TelemetryDAO();
};