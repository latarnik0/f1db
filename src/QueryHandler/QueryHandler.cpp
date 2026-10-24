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


// =====================================================================
//                             InventoryDAO
// =====================================================================
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

std::vector<Part> QueryHandler::getPartByPartName(const std::string& partName)
{
    return inventory.getPartByPartName(partName);
}

std::vector<Part> QueryHandler::getPartByCategory(const std::string& category)
{
    return inventory.getPartByCategory(category);
}

std::vector<Part> QueryHandler::getAllParts()
{
    return inventory.getAllParts();
}


// =====================================================================
//                             PersonnelDAO
// =====================================================================
// Drivers
int QueryHandler::addDriver(const std::string& firstName, const std::string& lastName, const int& experienceLvl)
{
    return personnel.addDriver(firstName, lastName, experienceLvl);
}

bool QueryHandler::deleteDriver(const int& driverId)
{
    return personnel.deleteDriver(driverId);
}

bool QueryHandler::updateDriverExperience(const int& driverId, const int& newLvl)
{
    return personnel.updateDriverExperience(driverId, newLvl);
}

Driver QueryHandler::getDriverById(const int& driverId)
{
    return personnel.getDriverById(driverId);
}

std::vector<Driver> QueryHandler::getAllDrivers()
{
    return personnel.getAllDrivers();
}

std::vector<Driver> QueryHandler::getDriverByLastName(const std::string& lastName)
{
    return personnel.getDriverByLastName(lastName);
}

std::vector<Driver> QueryHandler::getDriverByExpLvl(const int& experienceLvl)
{
    return personnel.getDriverByExpLvl(experienceLvl);
}

// Engineers
int QueryHandler::addEngineer(const std::string& firstName, const std::string& lastName, const std::string& divsion)
{
    return personnel.addEngineer(firstName, lastName, divsion);
}

bool QueryHandler::deleteEngineer(const int& engineerId)
{
    return personnel.deleteEngineer(engineerId);
}

Engineer QueryHandler::getEngineerById(const int& engineerId)
{
    return personnel.getEngineerById(engineerId);
}

std::vector<Engineer> QueryHandler::getAllEngineers()
{
    return personnel.getAllEngineers();
}

std::vector<Engineer> QueryHandler::getEngineerByLastName(const std::string& lastName)
{
    return personnel.getEngineerByLastName(lastName);
}

std::vector<Engineer> QueryHandler::getEngineerByDivision(const std::string& division)
{
    return personnel.getEngineerByDivision(division);
}


// =====================================================================
//                             TelemetryDAO
// =====================================================================
int QueryHandler::addSensor(const std::string& name, const std::string& unit)
{
    return telemetry.addSensor(name, unit);
}

std::vector<Sensor> QueryHandler::getAllSensors()
{
    return telemetry.getAllSensors();
}

int QueryHandler::startTelemetryRun(int sessionId, int vehicleId, int driverId)
{
    return telemetry.startTelemetryRun(sessionId, vehicleId, driverId);
}

bool QueryHandler::assignEngineerToRun(int runId, int engineerId)
{
    return telemetry.assignEngineerToRun(runId, engineerId);
}

void QueryHandler::insertMockReadings(std::vector<std::vector<SensorReading>>& sensorData)
{
    return telemetry.insertBatch(sensorData);
}


// =====================================================================
//                             SessionDAO
// =====================================================================
int QueryHandler::addTrack(const std::string& name, double lengthKm)
{
    return session.addTrack(name, lengthKm);
}    

bool QueryHandler::deleteTrack(int trackId)
{
    return session.deleteTrack(trackId);
}

std::vector<Track> QueryHandler::getAllTracks()
{
    return session.getAllTracks();
}

int QueryHandler::startSession(int trackId)
{
    return session.startSession(trackId);
}

std::vector<RaceSession> QueryHandler::getAllSessions()
{
    return session.getAllSessions();
}

SessionDetails QueryHandler::getSessionDetails(int sessionId)
{
    return session.getSessionDetails(sessionId);
}


// =====================================================================
//                             VehicleDAO
// =====================================================================
int QueryHandler::addVehicle(const std::string& chassisCode)
{
    return vehicle.addVehicle(chassisCode);
}

bool QueryHandler::deleteVehicle(int vehicleId)
{
    return vehicle.deleteVehicle(vehicleId);
}

std::vector<Vehicle> QueryHandler::getAllVehicles()
{
    return vehicle.getAllVehicles();
}

bool QueryHandler::setVehicleSetup(int vehicleId, double aeroBalance, const std::string& suspensionStiffness)
{
    return vehicle.setVehicleSetup(vehicleId, aeroBalance, suspensionStiffness);
}

VehicleSetup QueryHandler::getVehicleSetup(int vehicleId)
{
    return vehicle.getVehicleSetup(vehicleId);
}

bool QueryHandler::installPartOnVehicle(int vehicleId, int partId)
{
    return vehicle.installPartOnVehicle(vehicleId, partId);
}

bool QueryHandler::removePartFromVehicle(int vehicleId, int partId)
{
    return vehicle.removePartFromVehicle(vehicleId, partId);
}

std::vector<InstalledPart> QueryHandler::getVehicleWithInstalledParts(int vehicleId)
{
    return vehicle.getVehicleWithInstalledParts(vehicleId);
}