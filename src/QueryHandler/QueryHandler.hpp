#pragma once
#include "Mocks/sensor_readings.hpp"
#include "Logger/Logger.hpp"
#include "DAOs/Inventory.hpp"
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
    QueryHandler(sqlite3* db) : inventory(db), personnel(db), session(db), telemetry(db), vehicle(db) {}

    QueryHandler(sqlite3* db);
    ~QueryHandler();
};