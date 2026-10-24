#include "Models/Models.hpp"
#include "Logger/Logger.hpp"
#include "DAOs/VehicleDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"

VehicleDAO::VehicleDAO(sqlite3* db_connection) : db(db_connection) {}
VehicleDAO::~VehicleDAO() {}

int VehicleDAO::addVehicle(const std::string& chassisCode)
{
    const char* sql = "INSERT INTO vehicles (chassis_code) VALUES (?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, chassisCode.c_str(), -1, SQLITE_TRANSIENT);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
            Logger::getInstance().log(INFO, "Entry added");
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
                Logger::getInstance().log(INFO, "Entry deleted");
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
    const char* sql = "SELECT vehicle_id, chassis_code FROM vehicles;";
    sqlite3_stmt* stmt;
    std::vector<Vehicle> vehiclesList;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Vehicle v;
            v.id = sqlite3_column_int(stmt, 0);
            v.chassisCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
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

bool VehicleDAO::setVehicleSetup(int vehicleId, double aeroBalance, const std::string& suspensionStiffness)
{
    // INSERT OR REPLACE zadba o to, żeby zaktualizować wiersz, jeśli ustawienia dla tego bolidu już istnieją
    const char* sql = "INSERT OR REPLACE INTO vehicle_setups (vehicle_id, aero_balance, suspension_stiffness) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, vehicleId);
        sqlite3_bind_double(stmt, 2, aeroBalance);
        sqlite3_bind_text(stmt, 3, suspensionStiffness.c_str(), -1, SQLITE_TRANSIENT);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            result = true;
            Logger::getInstance().log(INFO, "Setup added");
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return result;
}

VehicleSetup VehicleDAO::getVehicleSetup(int vehicleId)
{
    const char* sql = "SELECT setup_id, vehicle_id, aero_balance, suspension_stiffness FROM vehicle_setups WHERE vehicle_id = ?;";
    sqlite3_stmt* stmt;
    VehicleSetup vs;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, vehicleId);

        if(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            vs.id = sqlite3_column_int(stmt, 0);
            vs.vehicleId = sqlite3_column_int(stmt, 1);
            vs.aeroBalance = sqlite3_column_double(stmt, 2);
            vs.suspensionStiffness = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
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
    // kolumna installed_at sama się wypełnia dzięki DEFAULT CURRENT_TIMESTAMP w schemacie bazy
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
            Logger::getInstance().log(INFO, "Part installed");
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
                Logger::getInstance().log(INFO, "Part removed");
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
    const char* sql = "SELECT p.part_id, p.part_name, p.category, vp.installed_at FROM vehicle_parts vp JOIN parts_inventory p ON vp.part_id = p.part_id WHERE vp.vehicle_id = ?;";
    sqlite3_stmt* stmt;
    std::vector<InstalledPart> installedPartsList = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, vehicleId);

        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            InstalledPart ip;
            ip.partId = sqlite3_column_int(stmt, 0);
            ip.partName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            ip.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            ip.installedAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
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