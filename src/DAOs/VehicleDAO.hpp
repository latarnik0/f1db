#pragma once
#include "Models/Models.hpp"
#include <sqlite3.h>
#include <string>
#include <vector>

class VehicleDAO
{
private:
    sqlite3* db;

public:
    VehicleDAO(sqlite3* db_connection);
    ~VehicleDAO();

    int addVehicle(const std::string& chassisCode, int driverId);
    bool deleteVehicle(int vehicleId);
    std::vector<Vehicle> getAllVehicles();
    bool setRunSetup(int runId, double frontWingAngle, double rearWingAngle, double brakeBias, double suspensionStiffness);
    VehicleSetup getRunSetup(int runId);
    bool installPartOnVehicle(int vehicleId, int partId);
    bool removePartFromVehicle(int vehicleId, int partId);
    std::vector<InstalledPart> getVehicleWithInstalledParts(int vehicleId);
};
