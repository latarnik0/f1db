#include "Logger/Logger.hpp"
#include "DAOs/TelemetryDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"

TelemetryDAO::TelemetryDAO(sqlite3* db_connection) : db(db_connection) {}
TelemetryDAO::~TelemetryDAO() {}


int TelemetryDAO::addSensor(const std::string& name, const std::string& unit)
{
    const char* sql = "INSERT INTO sensors (name, unit) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, unit.c_str(), -1, SQLITE_TRANSIENT);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
        }
    }
    else 
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return newId;
}

std::vector<Sensor> TelemetryDAO::getAllSensors()
{
    const char* sql = "SELECT sensor_id, name, unit FROM sensors;";
    sqlite3_stmt* stmt;
    std::vector<Sensor> sensorsList;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Sensor s;
            s.id = sqlite3_column_int(stmt, 0);
            s.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            s.unit = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            sensorsList.push_back(s);
        }
    }
    else 
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return sensorsList;
}

int TelemetryDAO::startTelemetryRun(int sessionId, int vehicleId, int driverId)
{
    const char* sql = "INSERT INTO telemetry_runs (session_id, vehicle_id, driver_id) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, sessionId);
        sqlite3_bind_int(stmt, 2, vehicleId);
        sqlite3_bind_int(stmt, 3, driverId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
        }
    }
    else 
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return newId;
}

bool TelemetryDAO::assignEngineerToRun(int runId, int engineerId)
{
    const char* sql = "INSERT INTO run_engineers (run_id, engineer_id) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, runId);
        sqlite3_bind_int(stmt, 2, engineerId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            result = true;
        }
    }
    else 
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return result;
}

void TelemetryDAO::insertBatch(std::vector<std::vector<SensorReading>>& sensorData)
{
    char* errorMessage = nullptr;
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errorMessage);

    const char* sql = "INSERT INTO sensor_readings (run_id, sensor_id, recorded_at, value) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    for(const auto& data : sensorData)
    {
        for(const auto& reading : data)
        {
            sqlite3_bind_int(stmt, 1, reading.runId);
            sqlite3_bind_int(stmt, 2, reading.sensorId);
            sqlite3_bind_text(stmt, 3, reading.recordedAt.c_str(), -1, SQLITE_TRANSIENT); 
            sqlite3_bind_double(stmt, 4, reading.value);
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errorMessage);
}