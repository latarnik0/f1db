#include "DatabaseManager/DatabaseManager.hpp"
#include "QueryHandler/QueryHandler.hpp"
#include "Mocks/MockReadings.hpp"
#include <iostream>
#include <vector>
#include <sqlite3.h>

QueryHandler::QueryHandler(sqlite3* db_connection) : 
    inventory(db_connection),
    personnel(db_connection),
    session(db_connection),
    telemetry(db_connection),
    vehicle(db_connection) {}

QueryHandler::~QueryHandler(){}

// PersonnelDAO
int QueryHandler::addDriver(const std::string& firstName, const std::string& lastName, int experienceLvl)
{
    return personnel.addDriver(firstName, lastName, experienceLvl);
}

int QueryHandler::addEngineer(const std::string& firstName, const std::string& lastName, const std::string& divsion)
{
    return personnel.addEngineer(firstName, lastName, divsion);
}

// =======================
// InventoryDAO
// =======================
int QueryHandler::addPart(const std::string& partName, const std::string& category)
{
    return inventory.addPart(partName, category);
}

bool QueryHandler::deletePart(const int& partId)
{
    return inventory.deletePart(partId);
}

Part QueryHandler::getPartById(const int& partId)
{
    return inventory.getPartById(partId);
}


// =======================
// TelemetryDAO
// =======================
void QueryHandler::insertMockReadings(std::vector<std::vector<SensorReading>>& sensorData)
{
    return telemetry.insertBatch(sensorData);
}

// SessionDAO
// VehicleDAO