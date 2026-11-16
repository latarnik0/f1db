#include "Models/Models.hpp"
#include "Logger/Logger.hpp"
#include "DAOs/VehicleDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"

VehicleDAO::VehicleDAO(sqlite3* db_connection) : db(db_connection) {}
VehicleDAO::~VehicleDAO() {}

int VehicleDAO::addVehicle(const std::string& chassisCode, int driverId)
{
    const char* sql = "INSERT INTO vehicles (chassis_code, driver_id) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, chassisCode.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, driverId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
            Logger::getInstance().log(INFO, "VEHICLE: Vehicle added");
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return newId;
}

bool VehicleDAO::deleteVehicle(int vehicleId)
{
    const char* sql = "DELETE FROM vehicles WHERE vehicle_id = ?;";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, vehicleId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            if(sqlite3_changes(db) > 0)
            {
                result = true;
                Logger::getInstance().log(INFO, "VEHICLE: Vehicle deleted");
            }
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return result;
}

std::vector<Vehicle> VehicleDAO::getAllVehicles()
{
    const char* sql = "SELECT vehicle_id, chassis_code, driver_id FROM vehicles;";
    sqlite3_stmt* stmt;
    std::vector<Vehicle> vehiclesList;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Vehicle v;
            v.id = sqlite3_column_int(stmt, 0);
            v.chassisCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            v.driverId = sqlite3_column_int(stmt, 2);
            vehiclesList.push_back(v);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return vehiclesList;
}

bool VehicleDAO::setRunSetup(int runId, double frontWingAngle, double rearWingAngle, double brakeBias, double suspensionStiffness)
{
    const char* sql = "INSERT OR REPLACE INTO vehicle_setups (run_id, wing_angle_front, wing_angle_rear, brake_bias, suspension_stiffness) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, runId);
        sqlite3_bind_double(stmt, 2, frontWingAngle);
        sqlite3_bind_double(stmt, 3, rearWingAngle);
        sqlite3_bind_double(stmt, 4, brakeBias);
        sqlite3_bind_double(stmt, 5, suspensionStiffness);

        if(sqlite3_step(stmt) == SQLITE_DONE)
        {
            result = true;
            Logger::getInstance().log(INFO, "VEHICLE: Setup added");
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return result;
}

VehicleSetup VehicleDAO::getRunSetup(int runId)
{
    const char* sql = "SELECT setup_id, run_id, wing_angle_front, wing_angle_rear, brake_bias, suspension_stiffness FROM vehicle_setups WHERE run_id = ?;";
    sqlite3_stmt* stmt;
    VehicleSetup vs = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, runId);

        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            vs.id = sqlite3_column_int(stmt, 0);
            vs.runId = sqlite3_column_int(stmt, 1);
            vs.frontWingAngle = sqlite3_column_double(stmt, 2);
            vs.rearWingAngle = sqlite3_column_double(stmt, 3);
            vs.brakeBias = sqlite3_column_double(stmt, 4);
            vs.suspensionStiffness = sqlite3_column_double(stmt, 5);
            sqlite3_finalize(stmt);
            Logger::getInstance().log(INFO, "Success");
            return vs;
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return vs;
}

bool VehicleDAO::installPartOnVehicle(int vehicleId, int partId)
{
    const char* sql = "INSERT INTO vehicle_parts (vehicle_id, part_id) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, vehicleId);
        sqlite3_bind_int(stmt, 2, partId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            result = true;
            Logger::getInstance().log(INFO, "VEHICLE: Part installed");
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return result;
}

bool VehicleDAO::removePartFromVehicle(int vehicleId, int partId)
{
    const char* sql = "DELETE FROM vehicle_parts WHERE vehicle_id = ? AND part_id = ?;";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, vehicleId);
        sqlite3_bind_int(stmt, 2, partId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            if(sqlite3_changes(db) > 0)
            {
                result = true;
                Logger::getInstance().log(INFO, "VEHICLE: Part removed");
            }
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return result;
}

std::vector<InstalledPart> VehicleDAO::getVehicleWithInstalledParts(int vehicleId)
{
    const char* sql = "SELECT p.part_id, p.name, p.category, vp.installation_date, p.wear_percentage FROM vehicle_parts vp JOIN parts_inventory p ON vp.part_id = p.part_id WHERE vp.vehicle_id = ?;";
    sqlite3_stmt* stmt;
    std::vector<InstalledPart> installedPartsList = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, vehicleId);

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            InstalledPart ip;
            ip.partId = sqlite3_column_int(stmt, 0);
            ip.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            ip.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const char* installedAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            ip.installedAt = installedAt != nullptr ? installedAt : "";
            ip.wearPercent = sqlite3_column_double(stmt, 4);
            installedPartsList.push_back(ip);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return installedPartsList;
}
