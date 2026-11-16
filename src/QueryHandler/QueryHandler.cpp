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
int QueryHandler::addPart(const std::string& name, const std::string& category, int stockQuantity, double wearPercent)
{
    return inventory.addPart(name, category, stockQuantity, wearPercent);
}

bool QueryHandler::updatePartWear(int partId, double wearPercent)
{
    return inventory.updatePartWear(partId, wearPercent);
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

std::vector<TelemetryRun> QueryHandler::getAllRuns()
{
    return telemetry.getAllRuns();
}

int QueryHandler::startTelemetryRun(int sessionId, int vehicleId)
{
    return telemetry.startTelemetryRun(sessionId, vehicleId);
}

bool QueryHandler::setRunSectors(int runId, double sector1S, double sector2S, double sector3S, double lapTimeS)
{
    return telemetry.setRunSectors(runId, sector1S, sector2S, sector3S, lapTimeS);
}

bool QueryHandler::assignEngineerToRun(int runId, int engineerId)
{
    return telemetry.assignEngineerToRun(runId, engineerId);
}

void QueryHandler::clearReadingsForRun(int runId)
{
    telemetry.clearReadingsForRun(runId);
}

void QueryHandler::insertMockReadings(std::vector<std::vector<MockSensorReading>>& sensorData)
{
    telemetry.insertBatch(sensorData);
}

// analytical data
RunDetails QueryHandler::getRunDetails(int runId)
{
    return telemetry.getRunDetails(runId);
}

std::vector<Engineer> QueryHandler::getEngineersForRun(int runId)
{
    return telemetry.getEngineersForRun(runId);
}

std::vector<SensorReading> QueryHandler::getRunTelemetry(int runId, int sensorId)
{
    return telemetry.getRunTelemetry(runId, sensorId);
}

SensorStatistics QueryHandler::getSensorStatistics(int runId, int sensorId)
{
    return telemetry.getSensorStatistics(runId, sensorId);
}


// =====================================================================
//                             SessionDAO
// =====================================================================
int QueryHandler::addTrack(const std::string& name, const std::string& location)
{
    return session.addTrack(name, location);
}    

bool QueryHandler::deleteTrack(int trackId)
{
    return session.deleteTrack(trackId);
}

std::vector<Track> QueryHandler::getAllTracks()
{
    return session.getAllTracks();
}

int QueryHandler::startSession(int trackId, const std::string& sessionType, const std::string& weather)
{
    return session.startSession(trackId, sessionType, weather);
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
int QueryHandler::addVehicle(const std::string& chassisCode, int driverId)
{
    return vehicle.addVehicle(chassisCode, driverId);
}

bool QueryHandler::deleteVehicle(int vehicleId)
{
    return vehicle.deleteVehicle(vehicleId);
}

std::vector<Vehicle> QueryHandler::getAllVehicles()
{
    return vehicle.getAllVehicles();
}

bool QueryHandler::setRunSetup(int runId, double frontWingAngle, double rearWingAngle, double brakeBias, double suspensionStiffness)
{
    return vehicle.setRunSetup(runId, frontWingAngle, rearWingAngle, brakeBias, suspensionStiffness);
}

VehicleSetup QueryHandler::getRunSetup(int runId)
{
    return vehicle.getRunSetup(runId);
}

SetupComparison QueryHandler::compareRunSetups(int runIdA, int runIdB)
{
    return vehicle.compareRunSetups(runIdA, runIdB);
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