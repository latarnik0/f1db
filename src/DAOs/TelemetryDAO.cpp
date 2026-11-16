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

std::vector<TelemetryRun> TelemetryDAO::getAllRuns()
{
    const char* sql =
        "SELECT tr.run_id, tr.session_id, tr.vehicle_id, v.driver_id, "
        "tr.sector_1, tr.sector_2, tr.sector_3, tr.lap_time, "
        "v.chassis_code, d.first_name, d.last_name "
        "FROM telemetry_runs tr "
        "JOIN vehicles v ON tr.vehicle_id = v.vehicle_id "
        "JOIN drivers d ON v.driver_id = d.driver_id "
        "ORDER BY tr.run_id DESC;";

    sqlite3_stmt* stmt;
    std::vector<TelemetryRun> runs;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            TelemetryRun run;
            run.id = sqlite3_column_int(stmt, 0);
            run.sessionId = sqlite3_column_int(stmt, 1);
            run.vehicleId = sqlite3_column_int(stmt, 2);
            run.driverId = sqlite3_column_int(stmt, 3);
            run.sector1S = sqlite3_column_double(stmt, 4);
            run.sector2S = sqlite3_column_double(stmt, 5);
            run.sector3S = sqlite3_column_double(stmt, 6);
            run.lapTimeS = sqlite3_column_double(stmt, 7);
            const unsigned char* chassis = sqlite3_column_text(stmt, 8);
            const unsigned char* first = sqlite3_column_text(stmt, 9);
            const unsigned char* last = sqlite3_column_text(stmt, 10);
            run.chassisCode = chassis ? reinterpret_cast<const char*>(chassis) : "";
            const std::string firstName = first ? reinterpret_cast<const char*>(first) : "";
            const std::string lastName = last ? reinterpret_cast<const char*>(last) : "";
            run.driverName = firstName + (firstName.empty() || lastName.empty() ? "" : " ") + lastName;
            runs.push_back(run);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return runs;
}

int TelemetryDAO::startTelemetryRun(int sessionId, int vehicleId)
{
    const char* sql = "INSERT INTO telemetry_runs (session_id, vehicle_id) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, sessionId);
        sqlite3_bind_int(stmt, 2, vehicleId);

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
    const char* sql = "UPDATE telemetry_runs SET sector_1 = ?, sector_2 = ?, sector_3 = ?, lap_time = ? WHERE run_id = ?;";
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

void TelemetryDAO::clearReadingsForRun(int runId)
{
    const char* sql = "DELETE FROM sensor_readings WHERE run_id = ?;";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, runId);
        if(sqlite3_step(stmt) == SQLITE_DONE)
        {
            Logger::getInstance().log(INFO, "TELEMETRY: Cleared readings for run");
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
}

void TelemetryDAO::insertBatch(std::vector<std::vector<MockSensorReading>>& sensorData)
{
    char* errorMessage = nullptr;
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errorMessage);

    const char* sql = "INSERT INTO sensor_readings (run_id, sensor_id, timestamp, value) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    for(const auto& data : sensorData)
    {
        for(const auto& reading : data)
        {
            sqlite3_bind_int(stmt, 1, reading.runId);
            sqlite3_bind_int(stmt, 2, reading.sensorId);
            sqlite3_bind_double(stmt, 3, reading.timestamp); 
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
        "tr.sector_1, tr.sector_2, tr.sector_3, tr.lap_time "
        "FROM telemetry_runs tr "
        "JOIN race_sessions rs ON tr.session_id = rs.session_id "
        "JOIN vehicles v ON tr.vehicle_id = v.vehicle_id "
        "JOIN drivers d ON v.driver_id = d.driver_id "
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
    const char* sql = "SELECT reading_id, timestamp, value FROM sensor_readings WHERE run_id = ? AND sensor_id = ? ORDER BY timestamp ASC;";
    sqlite3_stmt* stmt;
    std::vector<SensorReading> readings;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, runId);
        sqlite3_bind_int(stmt, 2, sensorId);

        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            SensorReading sr;
            sr.readingId = sqlite3_column_int(stmt, 0);
            sr.runId = runId;       
            sr.sensorId = sensorId; 
            sr.timestamp = sqlite3_column_double(stmt, 1);
            sr.value = sqlite3_column_double(stmt, 2);
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

std::vector<TrackPerformanceRow> TelemetryDAO::queryTrackPerformance(const std::string& trackName, double maxLapTime)
{
    const char* sql =
        "SELECT d.last_name, t.name as track_name, tr.lap_time, vs.wing_angle_front "
        "FROM telemetry_runs tr "
        "JOIN race_sessions rs ON tr.session_id = rs.session_id "
        "JOIN tracks t ON rs.track_id = t.track_id "
        "JOIN vehicles v ON tr.vehicle_id = v.vehicle_id "
        "JOIN drivers d ON v.driver_id = d.driver_id "
        "JOIN vehicle_setups vs ON tr.run_id = vs.run_id "
        "WHERE t.name = ? AND tr.lap_time < ? "
        "ORDER BY tr.lap_time ASC;";

    sqlite3_stmt* stmt;
    std::vector<TrackPerformanceRow> rows;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, trackName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 2, maxLapTime);

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            TrackPerformanceRow row;
            row.lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            row.trackName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            row.lapTime = sqlite3_column_double(stmt, 2);
            row.wingAngleFront = sqlite3_column_double(stmt, 3);
            rows.push_back(row);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return rows;
}

SensorWindowStatistics TelemetryDAO::getSensorWindowStats(int runId, const std::string& sensorName, double t0, double t1)
{
    const char* sql =
        "SELECT MAX(sr.value) as top_speed, MIN(sr.value) as apex_speed "
        "FROM sensor_readings sr "
        "JOIN sensors s ON sr.sensor_id = s.sensor_id "
        "JOIN telemetry_runs tr ON sr.run_id = tr.run_id "
        "WHERE tr.run_id = ? "
        "  AND s.name = ? "
        "  AND sr.timestamp BETWEEN ? AND ?;";

    sqlite3_stmt* stmt;
    SensorWindowStatistics stats = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, runId);
        sqlite3_bind_text(stmt, 2, sensorName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 3, t0);
        sqlite3_bind_double(stmt, 4, t1);

        if(sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_type(stmt, 0) != SQLITE_NULL)
        {
            stats.topSpeed = sqlite3_column_double(stmt, 0);
            stats.apexSpeed = sqlite3_column_double(stmt, 1);
            stats.hasData = true;
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return stats;
}
