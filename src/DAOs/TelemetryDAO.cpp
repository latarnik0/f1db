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
            Logger::getInstance().log(INFO, "TELEMETRY: Sensor added");
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
            Logger::getInstance().log(INFO, "TELEMETRY: New telemetry run added");
        }
    }
    else 
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return newId;
}

bool TelemetryDAO::setRunSectors(int runId, double sector1S, double sector2S, double sector3S, double lapTimeS)
{
    const char* sql = "UPDATE telemetry_runs SET sector1_s = ?, sector2_s = ?, sector3_s = ?, lap_time_s = ? WHERE run_id = ?;";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_double(stmt, 1, sector1S);
        sqlite3_bind_double(stmt, 2, sector2S);
        sqlite3_bind_double(stmt, 3, sector3S);
        sqlite3_bind_double(stmt, 4, lapTimeS);
        sqlite3_bind_int(stmt, 5, runId);

        if(sqlite3_step(stmt) == SQLITE_DONE)
        {
            if(sqlite3_changes(db) > 0)
            {
                result = true;
                Logger::getInstance().log(INFO, "TELEMETRY: Run sectors updated");
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
            Logger::getInstance().log(INFO, "TELEMETRY: Engineer assigned to a run");
        }
    }
    else 
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return result;
}

void TelemetryDAO::insertBatch(std::vector<std::vector<MockSensorReading>>& sensorData)
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
    Logger::getInstance().log(INFO, "TELEMETRY: Mock readings inserted");
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errorMessage);
}

RunDetails TelemetryDAO::getRunDetails(int runId)
{
    const char* sql =
        "SELECT tr.run_id, rs.session_date, v.chassis_code, d.first_name, d.last_name, "
        "tr.sector1_s, tr.sector2_s, tr.sector3_s, tr.lap_time_s "
        "FROM telemetry_runs tr "
        "JOIN race_sessions rs ON tr.session_id = rs.session_id "
        "JOIN vehicles v ON tr.vehicle_id = v.vehicle_id "
        "JOIN drivers d ON tr.driver_id = d.driver_id "
        "WHERE tr.run_id = ?;";

    sqlite3_stmt* stmt;
    RunDetails rd = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, runId);

        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            rd.runId = sqlite3_column_int(stmt, 0);
            rd.sessionDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            rd.chassisCode = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            rd.driverFirstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            rd.driverLastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            rd.sector1S = sqlite3_column_double(stmt, 5);
            rd.sector2S = sqlite3_column_double(stmt, 6);
            rd.sector3S = sqlite3_column_double(stmt, 7);
            rd.lapTimeS = sqlite3_column_double(stmt, 8);
            sqlite3_finalize(stmt);
            Logger::getInstance().log(INFO, "TELEMETRY: getRunDetails success");
            return rd;
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return rd;
}

std::vector<Engineer> TelemetryDAO::getEngineersForRun(int runId)
{
    const char* sql = 
        "SELECT e.engineer_id, e.first_name, e.last_name, e.division "
        "FROM engineers e "
        "JOIN run_engineers re ON e.engineer_id = re.engineer_id "
        "WHERE re.run_id = ?;";
        
    sqlite3_stmt* stmt;
    std::vector<Engineer> engineersList;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, runId);

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

std::vector<SensorReading> TelemetryDAO::getRunTelemetry(int runId, int sensorId)
{
    const char* sql = "SELECT recorded_at, value FROM sensor_readings WHERE run_id = ? AND sensor_id = ? ORDER BY recorded_at ASC;";
    sqlite3_stmt* stmt;
    std::vector<SensorReading> readings;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, runId);
        sqlite3_bind_int(stmt, 2, sensorId);

        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            SensorReading sr;
            sr.runId = runId;       
            sr.sensorId = sensorId; 
            sr.recordedAt = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            sr.value = sqlite3_column_double(stmt, 1);
            readings.push_back(sr);
        }
    }
    else 
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return readings;
}

SensorStatistics TelemetryDAO::getSensorStatistics(int runId, int sensorId)
{
    const char* sql = 
        "SELECT MAX(value), MIN(value), AVG(value) "
        "FROM sensor_readings "
        "WHERE run_id = ? AND sensor_id = ? "
        "GROUP BY run_id, sensor_id;";
        
    sqlite3_stmt* stmt;
    SensorStatistics stats;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, runId);
        sqlite3_bind_int(stmt, 2, sensorId);

        if(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            stats.runId = runId;
            stats.sensorId = sensorId;
            stats.maxValue = sqlite3_column_double(stmt, 0);
            stats.minValue = sqlite3_column_double(stmt, 1);
            stats.avgValue = sqlite3_column_double(stmt, 2);
            sqlite3_finalize(stmt);
            return stats;
        }
    }
    else 
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return stats;
}