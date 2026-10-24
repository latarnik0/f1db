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

    int addVehicle(const std::string& chassisCode);
    bool deleteVehicle(int vehicleId);
    std::vector<Vehicle> getAllVehicles();
    bool setVehicleSetup(int vehicleId, double aeroBalance, const std::string& suspensionStiffness);
    VehicleSetup getVehicleSetup(int vehicleId);
    bool installPartOnVehicle(int vehicleId, int partId);
    bool removePartFromVehicle(int vehicleId, int partId);
    std::vector<InstalledPart> getVehicleWithInstalledParts(int vehicleId);
};