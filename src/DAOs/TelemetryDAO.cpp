#include "DAOs/TelemetryDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"

TelemetryDAO::TelemetryDAO(sqlite3* db_connection) : db(db_connection)
{

}

TelemetryDAO::~TelemetryDAO()
{
    
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