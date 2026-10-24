#pragma once
#include "Models/Models.hpp"
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

    // =================
    // InventoryDAO
    // =================
    int addPart(const std::string& partName, const std::string& category);
    bool deletePart(const int& partId);
    Part getPartById(const int& partId);
    std::vector<Part> getPartByPartName(const std::string& partName);
    std::vector<Part> getPartByCategory(const std::string& category);
    std::vector<Part> getAllParts();

    // =================
    // PersonnelDAO
    // =================
    // Drivers
    int addDriver(const std::string& firstName, const std::string& lastName, const int& experienceLvl);
    bool deleteDriver(const int& driverId);
    Driver getDriverById(const int& driverId);
    std::vector<Driver> getAllDrivers();
    std::vector<Driver> getDriverByLastName(const std::string& lastName);
    std::vector<Driver> getDriverByExpLvl(const int& experienceLvl);
    bool updateDriverExperience(const int& driverId, const int& newLvl);

    // Engineers
    int addEngineer(const std::string& firstName, const std::string& lastName, const std::string& division);
    bool deleteEngineer(const int& engineerId);
    Engineer getEngineerById(const int& engineerId);
    std::vector<Engineer> getAllEngineers();
    std::vector<Engineer> getEngineerByLastName(const std::string& lastName);
    std::vector<Engineer> getEngineerByDivision(const std::string& division);

    // =================
    // TelemetryDAO
    // =================
    void insertMockReadings(std::vector<std::vector<SensorReading>>& sensorData);


    // =================
    // SessionDAO
    // =================
    int addTrack(const std::string& name, double lengthKm);    
    std::vector<Track> getAllTracks();
    bool deleteTrack(int trackId);
    int startSession(int trackId);
    std::vector<RaceSession> getAllSessions();
    SessionDetails getSessionDetails(int sessionId);


    // =================
    // VehicleDAO
    // =================
    int addVehicle(const std::string& chassisCode);
    bool deleteVehicle(int vehicleId);
    std::vector<Vehicle> getAllVehicles();
    bool setVehicleSetup(int vehicleId, double aeroBalance, const std::string& suspensionStiffness);
    VehicleSetup getVehicleSetup(int vehicleId);
    bool installPartOnVehicle(int vehicleId, int partId);
    bool removePartFromVehicle(int vehicleId, int partId);
    std::vector<InstalledPart> getVehicleWithInstalledParts(int vehicleId);
};