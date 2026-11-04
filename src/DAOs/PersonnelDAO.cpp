#include "Models/Models.hpp"
#include "Logger/Logger.hpp" 
#include "DAOs/PersonnelDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"
#include <string>
#include <vector>

PersonnelDAO::PersonnelDAO(sqlite3* db_connection) : db(db_connection) {}
PersonnelDAO::~PersonnelDAO() {}

// ==================
// Drivers
// ==================
int PersonnelDAO::addDriver(const std::string& firstName, const std::string& lastName, const int& experienceLvl)
{
    const char* sql = "INSERT INTO drivers (first_name, last_name, experience_lvl) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, experienceLvl);
        
        // Poprawiono: usunięto podwójne wywołanie sqlite3_step
        if (sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
            Logger::getInstance().log(INFO, "PERSONNEL: Driver added");
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return newId;
}

Driver PersonnelDAO::getDriverById(const int& driverId)
{
    const char* sql = "SELECT driver_id, first_name, last_name, experience_lvl FROM drivers WHERE driver_id = ?;";
    sqlite3_stmt* stmt;
    Driver d = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, driverId);

        if(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            d.id = sqlite3_column_int(stmt, 0);
            d.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            d.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            d.experienceLvl = sqlite3_column_int(stmt, 3);  
            sqlite3_finalize(stmt);
            return d;
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return d;
}

std::vector<Driver> PersonnelDAO::getAllDrivers()
{
    const char* sql = "SELECT driver_id, first_name, last_name, experience_lvl FROM drivers;";
    sqlite3_stmt* stmt;
    std::vector<Driver> driversList;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Driver d;
            d.id = sqlite3_column_int(stmt, 0);
            d.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            d.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            d.experienceLvl = sqlite3_column_int(stmt, 3);
            driversList.push_back(d);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return driversList;
}

bool PersonnelDAO::updateDriverExperience(const int& driverId, const int& newLvl)
{
    const char* sql = "UPDATE drivers SET experience_lvl = ? WHERE driver_id = ?;";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, newLvl);
        sqlite3_bind_int(stmt, 2, driverId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            if(sqlite3_changes(db) > 0)
            {
                result = true;
                Logger::getInstance().log(INFO, "PERSONNEL: Driver's experience updated");
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

bool PersonnelDAO::deleteDriver(const int& driverId)
{
    const char* sql = "DELETE FROM drivers WHERE driver_id = ?;";
    sqlite3_stmt* stmt;
    bool success = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, driverId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            if(sqlite3_changes(db) > 0)
            {
                success = true;
                Logger::getInstance().log(INFO, "PERSONNEL: Driver deleted");
            }
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return success;
}

std::vector<Driver> PersonnelDAO::getDriverByLastName(const std::string& lastName)
{
    const char* sql = "SELECT driver_id, first_name, last_name, experience_lvl FROM drivers WHERE last_name = ?;";
    sqlite3_stmt* stmt;
    std::vector<Driver> driversList;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, lastName.c_str(), -1, SQLITE_TRANSIENT);
        while (sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Driver d;
            d.id = sqlite3_column_int(stmt, 0);
            d.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            d.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            d.experienceLvl = sqlite3_column_int(stmt, 3);
            driversList.push_back(d);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return driversList;
}

std::vector<Driver> PersonnelDAO::getDriverByExpLvl(const int& experienceLvl)
{
    const char* sql = "SELECT driver_id, first_name, last_name, experience_lvl FROM drivers WHERE experience_lvl = ?;";
    sqlite3_stmt* stmt;
    std::vector<Driver> driversList;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, experienceLvl);
        while (sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Driver d;
            d.id = sqlite3_column_int(stmt, 0);
            d.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            d.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            d.experienceLvl = sqlite3_column_int(stmt, 3);
            driversList.push_back(d);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return driversList;
}

// ==================
// Engineers
// ==================
int PersonnelDAO::addEngineer(const std::string& firstName, const std::string& lastName, const std::string& division)
{
    const char* sql = "INSERT INTO engineers (first_name, last_name, division) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, division.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
            Logger::getInstance().log(INFO, "PERSONNEL: Engineer added");
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return newId;
}

Engineer PersonnelDAO::getEngineerById(const int& engineerId)
{
    const char* sql = "SELECT engineer_id, first_name, last_name, division FROM engineers WHERE engineer_id = ?;";
    sqlite3_stmt* stmt;
    Engineer e = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, engineerId);

        if(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            e.id = sqlite3_column_int(stmt, 0);
            e.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            e.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            e.division = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            
            sqlite3_finalize(stmt);
            return e;
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return e;
}

std::vector<Engineer> PersonnelDAO::getAllEngineers()
{
    const char* sql = "SELECT engineer_id, first_name, last_name, division FROM engineers;";
    sqlite3_stmt* stmt;
    std::vector<Engineer> engineersList;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Engineer e;
            e.id = sqlite3_column_int(stmt, 0);
            e.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            e.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            e.division = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));      
            engineersList.push_back(e);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return engineersList;
}

bool PersonnelDAO::deleteEngineer(const int& engineerId)
{
    const char* sql = "DELETE FROM engineers WHERE engineer_id = ?;";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, engineerId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            if(sqlite3_changes(db) > 0)
            {
                result = true;
                Logger::getInstance().log(INFO, "PERSONNEL: Engineer deleted");
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

std::vector<Engineer> PersonnelDAO::getEngineerByLastName(const std::string& lastName)
{
    const char* sql = "SELECT engineer_id, first_name, last_name, division FROM engineers WHERE last_name = ?;";
    sqlite3_stmt* stmt;
    std::vector<Engineer> engineersList;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, lastName.c_str(), -1, SQLITE_TRANSIENT);
        while (sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Engineer e;
            e.id = sqlite3_column_int(stmt, 0);
            e.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            e.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            e.division = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            engineersList.push_back(e);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return engineersList;
}

std::vector<Engineer> PersonnelDAO::getEngineerByDivision(const std::string& division)
{
    const char* sql = "SELECT engineer_id, first_name, last_name, division FROM engineers WHERE division = ?;";
    sqlite3_stmt* stmt;
    std::vector<Engineer> engineersList;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, division.c_str(), -1, SQLITE_TRANSIENT);
        while (sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Engineer e;
            e.id = sqlite3_column_int(stmt, 0);
            e.firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            e.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            e.division = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            engineersList.push_back(e);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return engineersList;
}