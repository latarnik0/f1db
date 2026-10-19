#pragma once
#include "Mocks/sensor_readings.hpp"
#include "Logger/Logger.hpp"
#include "DAOs/Inventory.hpp"
#include "DAOs/PersonnelDAO.hpp"
#include "DAOs/SessionDAO.hpp"
#include "DAOs/TelemetryDAO.hpp"
#include <iostream>
#include <sqlite3.h>

class QueryHandler
{
private:
    //Telemetry telemetry;
    //Driver drivers;
public:
    QueryHandler(sqlite3* db);
    ~QueryHandler();
};