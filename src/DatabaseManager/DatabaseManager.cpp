#include "DatabaseManager/DatabaseManager.hpp"
#include <iostream>
#include <sqlite3.h>
#include <string>

DatabaseManager::DatabaseManager(const char* db_path)
{
    if(sqlite3_open(db_path, &db) != SQLITE_OK)
    {
        Logger::getInstance().log(CRITICAL, "DBMAN: Cannot open/create database!\n");
        sqlite3_close(db);
        db = nullptr;
    }
    else
    {
        Logger::getInstance().log(INFO, "DBMAN: Connected with database.\n");
        migrateSchema();
    }
}

DatabaseManager::~DatabaseManager()
{
    if(db)
    {
        sqlite3_close(db);
        Logger::getInstance().log(INFO, "DBMAN: Closed connection with database.\n");
    }
}

sqlite3* DatabaseManager::getConnection()
{
    return db;
}

bool DatabaseManager::isConnected() const
{
    return db != nullptr;
}

bool DatabaseManager::tableHasColumn(const std::string& tableName, const std::string& columnName)
{
    const std::string sql = "PRAGMA table_info(" + tableName + ");";
    sqlite3_stmt* stmt = nullptr;
    bool found = false;

    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
        return false;
    }

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        if(name != nullptr && columnName == name)
        {
            found = true;
            break;
        }
    }

    sqlite3_finalize(stmt);
    return found;
}

void DatabaseManager::addColumnIfMissing(const std::string& tableName, const std::string& columnName, const std::string& columnDef)
{
    if(tableHasColumn(tableName, columnName))
    {
        return;
    }

    const std::string sql = "ALTER TABLE " + tableName + " ADD COLUMN " + columnName + " " + columnDef + ";";
    char* errMsg = nullptr;

    if(sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string message = "DBMAN: Migration failed for " + tableName + "." + columnName;
        if(errMsg != nullptr)
        {
            message += ": ";
            message += errMsg;
            sqlite3_free(errMsg);
        }
        Logger::getInstance().log(ERROR, message);
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Migrated " + tableName + "." + columnName);
}

void DatabaseManager::migrateSchema()
{
    addColumnIfMissing("parts_inventory", "wear_percent", "REAL NOT NULL DEFAULT 0");
    addColumnIfMissing("vehicle_setups", "front_wing_angle", "REAL NOT NULL DEFAULT 0");
    addColumnIfMissing("vehicle_setups", "rear_wing_angle", "REAL NOT NULL DEFAULT 0");
    addColumnIfMissing("vehicle_setups", "brake_bias", "REAL NOT NULL DEFAULT 50");
    addColumnIfMissing("race_sessions", "weather_condition", "TEXT NOT NULL DEFAULT ''");
    addColumnIfMissing("race_sessions", "temperature_c", "REAL NOT NULL DEFAULT 0");
    addColumnIfMissing("telemetry_runs", "sector1_s", "REAL NOT NULL DEFAULT 0");
    addColumnIfMissing("telemetry_runs", "sector2_s", "REAL NOT NULL DEFAULT 0");
    addColumnIfMissing("telemetry_runs", "sector3_s", "REAL NOT NULL DEFAULT 0");
    addColumnIfMissing("telemetry_runs", "lap_time_s", "REAL NOT NULL DEFAULT 0");
}
