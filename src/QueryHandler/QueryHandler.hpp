#pragma once
#include "Logger/Logger.hpp"
#include "DAOs/InventoryDAO.hpp"
#include "DAOs/PersonnelDAO.hpp"
#include "DAOs/SessionDAO.hpp"
#include "DAOs/TelemetryDAO.hpp"
#include "DAOs/VehicleDAO.hpp"
#include <iostream>
#include <sqlite3.h>

class QueryHandler
{
private:
    InventoryDAO inventory;
    PersonnelDAO personnel;
    SessionDAO session;
    TelemetryDAO telemetry;
    VehicleDAO vehicle;

public:
    QueryHandler(sqlite3* db_connection);
    ~QueryHandler();

    // PersonnelDAO
    int addDriver(const std::string& firstName, const std::string& lastName, int experienceLvl);
    int addEngineer(const std::string& firstName, const std::string& lastName, const std::string& division);

    // InventoryDAO
    int addPart(const std::string& partName, const std::string& partType);

    // SessionDAO

    // TelemetryDAO
    void insertMockReadings(std::vector<std::vector<SensorReading>>& sensorData);

    // VehicleDAO
};