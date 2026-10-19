#pragma once
#include "QueryHandler/QueryHandler.hpp"

class TelemetryDAO
{
private:
    sqlite3* db;

public:
    TelemetryDAO(sqlite3* db);
    ~TelemetryDAO();
};