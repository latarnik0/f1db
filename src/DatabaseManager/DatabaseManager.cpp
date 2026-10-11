#include "DatabaseManager/DatabaseManager.hpp"
#include <iostream>
#include <sqlite3.h>

DatabaseManager::DatabaseManager(const char* db_path)
{
    if(sqlite3_open(db_path, &db) != SQLITE_OK)
    {
        Logger::getInstance().log(CRITICAL, "Cannot open/create database!\n");
        sqlite3_close(db); 
    } 
    else 
    {
        Logger::getInstance().log(INFO, "Connected with database.\n");
    }
}
    
DatabaseManager::~DatabaseManager() 
{
    if(db) 
    {
        sqlite3_close(db);
        Logger::getInstance().log(INFO, "Closed connection with database.\n");
    }
}

void DatabaseManager::insertMockReadings()
{
    std::vector<std::vector<SensorReading>> sensorData = prepareMockReadings();
    char* errorMessage = nullptr;
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errorMessage);

    // query prep
    const char* sql = "INSERT INTO sensor_readings (telemetry_run_id, sensor_id, recorded_at, value) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    for (const auto& data : sensorData)
    {
        for (const auto& reading : data)
        {
            // vector values --> sql vars
            sqlite3_bind_int(stmt, 1, reading.runId);
            sqlite3_bind_int(stmt, 2, reading.sensorId);
            sqlite3_bind_text(stmt, 3, reading.recordedAt.c_str(), -1, SQLITE_TRANSIENT); 
            sqlite3_bind_double(stmt, 4, reading.value);

            // insert to RAM
            sqlite3_step(stmt);

            // reset before another reading
            sqlite3_reset(stmt);
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errorMessage);
}
