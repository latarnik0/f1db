#pragma once
#include <sqlite3.h>
#include "Mocks/MockReadings.hpp"

class TelemetryDAO
{
private:
    sqlite3* db;

public:
    TelemetryDAO(sqlite3* db_connection);
    ~TelemetryDAO();
    void insertBatch(std::vector<std::vector<SensorReading>>& sensorData);
};