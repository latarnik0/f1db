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
        if(!execSql("PRAGMA foreign_keys = ON;"))
        {
            Logger::getInstance().log(ERROR, "DBMAN: Failed to enable foreign_keys");
        }
        ensureSchema();
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

bool DatabaseManager::execSql(const std::string& sql)
{
    char* errMsg = nullptr;
    if(sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string message = "DBMAN: SQL failed";
        if(errMsg != nullptr)
        {
            message += ": ";
            message += errMsg;
            sqlite3_free(errMsg);
        }
        Logger::getInstance().log(ERROR, message);
        return false;
    }
    return true;
}

bool DatabaseManager::tableExists(const std::string& tableName)
{
    const std::string sql = "SELECT 1 FROM sqlite_master WHERE type='table' AND name='" + tableName + "' LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;
    bool found = false;

    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
        return false;
    }

    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        found = true;
    }

    sqlite3_finalize(stmt);
    return found;
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
    if(!execSql(sql))
    {
        Logger::getInstance().log(ERROR, "DBMAN: Migration failed for " + tableName + "." + columnName);
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Migrated " + tableName + "." + columnName);
}

void DatabaseManager::ensureSchema()
{
    execSql(
        "CREATE TABLE IF NOT EXISTS drivers ("
        "driver_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "first_name TEXT NOT NULL,"
        "last_name TEXT NOT NULL,"
        "experience_lvl INTEGER NOT NULL DEFAULT 1 CHECK(experience_lvl >= 1 AND experience_lvl <= 5)"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS engineers ("
        "engineer_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "first_name TEXT NOT NULL,"
        "last_name TEXT NOT NULL,"
        "division TEXT NOT NULL"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS tracks ("
        "track_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "location TEXT NOT NULL"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS sensors ("
        "sensor_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "unit TEXT NOT NULL"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS vehicles ("
        "vehicle_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "chassis_code TEXT NOT NULL UNIQUE,"
        "driver_id INTEGER NOT NULL REFERENCES drivers(driver_id)"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS parts_inventory ("
        "part_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "category TEXT NOT NULL,"
        "stock_quantity INTEGER NOT NULL DEFAULT 0,"
        "wear_percentage REAL NOT NULL DEFAULT 0"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS vehicle_parts ("
        "vehicle_id INTEGER NOT NULL REFERENCES vehicles(vehicle_id) ON DELETE CASCADE,"
        "part_id INTEGER NOT NULL REFERENCES parts_inventory(part_id) ON DELETE CASCADE,"
        "installation_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "PRIMARY KEY(vehicle_id, part_id)"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS race_sessions ("
        "session_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "track_id INTEGER NOT NULL REFERENCES tracks(track_id),"
        "session_type TEXT NOT NULL DEFAULT '',"
        "session_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "weather TEXT NOT NULL DEFAULT ''"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS telemetry_runs ("
        "run_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "session_id INTEGER NOT NULL REFERENCES race_sessions(session_id) ON DELETE CASCADE,"
        "vehicle_id INTEGER NOT NULL REFERENCES vehicles(vehicle_id),"
        "lap_time REAL NOT NULL DEFAULT 0,"
        "sector_1 REAL NOT NULL DEFAULT 0,"
        "sector_2 REAL NOT NULL DEFAULT 0,"
        "sector_3 REAL NOT NULL DEFAULT 0"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS vehicle_setups ("
        "setup_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "run_id INTEGER NOT NULL UNIQUE REFERENCES telemetry_runs(run_id) ON DELETE CASCADE,"
        "wing_angle_front REAL NOT NULL DEFAULT 0,"
        "wing_angle_rear REAL NOT NULL DEFAULT 0,"
        "brake_bias REAL NOT NULL DEFAULT 50,"
        "suspension_stiffness REAL NOT NULL DEFAULT 0"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS run_engineers ("
        "run_id INTEGER NOT NULL REFERENCES telemetry_runs(run_id) ON DELETE CASCADE,"
        "engineer_id INTEGER NOT NULL REFERENCES engineers(engineer_id) ON DELETE CASCADE,"
        "PRIMARY KEY(run_id, engineer_id)"
        ");"
    );

    execSql(
        "CREATE TABLE IF NOT EXISTS sensor_readings ("
        "reading_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "run_id INTEGER NOT NULL REFERENCES telemetry_runs(run_id) ON DELETE CASCADE,"
        "sensor_id INTEGER NOT NULL REFERENCES sensors(sensor_id),"
        "timestamp REAL NOT NULL,"
        "value REAL NOT NULL"
        ");"
    );
}

void DatabaseManager::ensureIndexes()
{
    execSql("CREATE INDEX IF NOT EXISTS idx_sensor_readings_run_sensor_ts ON sensor_readings(run_id, sensor_id, timestamp);");
    execSql("CREATE INDEX IF NOT EXISTS idx_telemetry_runs_session ON telemetry_runs(session_id);");
    execSql("CREATE INDEX IF NOT EXISTS idx_race_sessions_track ON race_sessions(track_id);");
}

bool DatabaseManager::needsLegacyMigration()
{
    if(tableExists("tracks") && tableHasColumn("tracks", "length_km"))
    {
        return true;
    }
    if(tableExists("vehicles") && !tableHasColumn("vehicles", "driver_id"))
    {
        return true;
    }
    if(tableExists("parts_inventory") && tableHasColumn("parts_inventory", "part_name"))
    {
        return true;
    }
    if(tableExists("vehicle_parts") && tableHasColumn("vehicle_parts", "installed_at"))
    {
        return true;
    }
    if(tableExists("race_sessions") && tableHasColumn("race_sessions", "weather_condition"))
    {
        return true;
    }
    if(tableExists("telemetry_runs") && tableHasColumn("telemetry_runs", "sector1_s"))
    {
        return true;
    }
    if(tableExists("vehicle_setups") && tableHasColumn("vehicle_setups", "vehicle_id"))
    {
        return true;
    }
    if(tableExists("sensor_readings") && tableHasColumn("sensor_readings", "recorded_at"))
    {
        return true;
    }
    return false;
}

void DatabaseManager::ensurePlaceholderDriverIfNeeded()
{
    if(!tableExists("vehicles") || !tableExists("drivers"))
    {
        return;
    }
    if(tableHasColumn("vehicles", "driver_id"))
    {
        return;
    }

    execSql(
        "INSERT INTO drivers (first_name, last_name, experience_lvl) "
        "SELECT 'Unknown', 'Driver', 1 "
        "WHERE (SELECT COUNT(*) FROM drivers) = 0 "
        "AND (SELECT COUNT(*) FROM vehicles) > 0;"
    );
}

void DatabaseManager::rebuildTracksIfNeeded()
{
    if(!tableExists("tracks") || !tableHasColumn("tracks", "length_km"))
    {
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Rebuilding tracks");
    if(!execSql(
        "CREATE TABLE tracks_new ("
        "track_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "location TEXT NOT NULL"
        ");"
    ))
    {
        return;
    }

    if(!execSql(
        "INSERT INTO tracks_new (track_id, name, location) "
        "SELECT track_id, name, COALESCE(CAST(length_km AS TEXT), '') FROM tracks;"
    ))
    {
        execSql("DROP TABLE IF EXISTS tracks_new;");
        return;
    }

    if(!execSql("DROP TABLE tracks;"))
    {
        return;
    }
    execSql("ALTER TABLE tracks_new RENAME TO tracks;");
}

void DatabaseManager::rebuildPartsInventoryIfNeeded()
{
    if(!tableExists("parts_inventory") || !tableHasColumn("parts_inventory", "part_name"))
    {
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Rebuilding parts_inventory");
    if(!execSql(
        "CREATE TABLE parts_inventory_new ("
        "part_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "category TEXT NOT NULL,"
        "stock_quantity INTEGER NOT NULL DEFAULT 0,"
        "wear_percentage REAL NOT NULL DEFAULT 0"
        ");"
    ))
    {
        return;
    }

    std::string insertSql =
        "INSERT INTO parts_inventory_new (part_id, name, category, stock_quantity, wear_percentage) "
        "SELECT part_id, part_name, category, 0, ";
    if(tableHasColumn("parts_inventory", "wear_percent"))
    {
        insertSql += "COALESCE(wear_percent, 0) FROM parts_inventory;";
    }
    else
    {
        insertSql += "0 FROM parts_inventory;";
    }

    if(!execSql(insertSql))
    {
        execSql("DROP TABLE IF EXISTS parts_inventory_new;");
        return;
    }

    if(!execSql("DROP TABLE parts_inventory;"))
    {
        return;
    }
    execSql("ALTER TABLE parts_inventory_new RENAME TO parts_inventory;");
}

void DatabaseManager::rebuildVehiclePartsIfNeeded()
{
    if(!tableExists("vehicle_parts") || !tableHasColumn("vehicle_parts", "installed_at"))
    {
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Rebuilding vehicle_parts");
    if(!execSql(
        "CREATE TABLE vehicle_parts_new ("
        "vehicle_id INTEGER NOT NULL REFERENCES vehicles(vehicle_id) ON DELETE CASCADE,"
        "part_id INTEGER NOT NULL REFERENCES parts_inventory(part_id) ON DELETE CASCADE,"
        "installation_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "PRIMARY KEY(vehicle_id, part_id)"
        ");"
    ))
    {
        return;
    }

    if(!execSql(
        "INSERT INTO vehicle_parts_new (vehicle_id, part_id, installation_date) "
        "SELECT vehicle_id, part_id, installed_at FROM vehicle_parts;"
    ))
    {
        execSql("DROP TABLE IF EXISTS vehicle_parts_new;");
        return;
    }

    if(!execSql("DROP TABLE vehicle_parts;"))
    {
        return;
    }
    execSql("ALTER TABLE vehicle_parts_new RENAME TO vehicle_parts;");
}

void DatabaseManager::rebuildRaceSessionsIfNeeded()
{
    if(!tableExists("race_sessions") || !tableHasColumn("race_sessions", "weather_condition"))
    {
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Rebuilding race_sessions");
    if(!execSql(
        "CREATE TABLE race_sessions_new ("
        "session_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "track_id INTEGER NOT NULL REFERENCES tracks(track_id),"
        "session_type TEXT NOT NULL DEFAULT '',"
        "session_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "weather TEXT NOT NULL DEFAULT ''"
        ");"
    ))
    {
        return;
    }

    if(!execSql(
        "INSERT INTO race_sessions_new (session_id, track_id, session_type, session_date, weather) "
        "SELECT session_id, track_id, '', session_date, COALESCE(weather_condition, '') FROM race_sessions;"
    ))
    {
        execSql("DROP TABLE IF EXISTS race_sessions_new;");
        return;
    }

    if(!execSql("DROP TABLE race_sessions;"))
    {
        return;
    }
    execSql("ALTER TABLE race_sessions_new RENAME TO race_sessions;");
}

void DatabaseManager::rebuildVehiclesIfNeeded()
{
    if(!tableExists("vehicles") || tableHasColumn("vehicles", "driver_id"))
    {
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Rebuilding vehicles");
    ensurePlaceholderDriverIfNeeded();

    if(!execSql(
        "CREATE TABLE vehicles_new ("
        "vehicle_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "chassis_code TEXT NOT NULL UNIQUE,"
        "driver_id INTEGER NOT NULL REFERENCES drivers(driver_id)"
        ");"
    ))
    {
        return;
    }

    if(!execSql(
        "INSERT INTO vehicles_new (vehicle_id, chassis_code, driver_id) "
        "SELECT v.vehicle_id, v.chassis_code, "
        "COALESCE("
        "(SELECT tr.driver_id FROM telemetry_runs tr WHERE tr.vehicle_id = v.vehicle_id ORDER BY tr.run_id DESC LIMIT 1),"
        "(SELECT MIN(driver_id) FROM drivers)"
        ") "
        "FROM vehicles v;"
    ))
    {
        execSql("DROP TABLE IF EXISTS vehicles_new;");
        return;
    }

    if(!execSql("DROP TABLE vehicles;"))
    {
        return;
    }
    execSql("ALTER TABLE vehicles_new RENAME TO vehicles;");
}

void DatabaseManager::rebuildTelemetryRunsIfNeeded()
{
    if(!tableExists("telemetry_runs") || !tableHasColumn("telemetry_runs", "sector1_s"))
    {
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Rebuilding telemetry_runs");
    if(!execSql(
        "CREATE TABLE telemetry_runs_new ("
        "run_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "session_id INTEGER NOT NULL REFERENCES race_sessions(session_id) ON DELETE CASCADE,"
        "vehicle_id INTEGER NOT NULL REFERENCES vehicles(vehicle_id),"
        "lap_time REAL NOT NULL DEFAULT 0,"
        "sector_1 REAL NOT NULL DEFAULT 0,"
        "sector_2 REAL NOT NULL DEFAULT 0,"
        "sector_3 REAL NOT NULL DEFAULT 0"
        ");"
    ))
    {
        return;
    }

    if(!execSql(
        "INSERT INTO telemetry_runs_new (run_id, session_id, vehicle_id, lap_time, sector_1, sector_2, sector_3) "
        "SELECT run_id, session_id, vehicle_id, "
        "COALESCE(lap_time_s, 0), COALESCE(sector1_s, 0), COALESCE(sector2_s, 0), COALESCE(sector3_s, 0) "
        "FROM telemetry_runs;"
    ))
    {
        execSql("DROP TABLE IF EXISTS telemetry_runs_new;");
        return;
    }

    if(!execSql("DROP TABLE telemetry_runs;"))
    {
        return;
    }
    execSql("ALTER TABLE telemetry_runs_new RENAME TO telemetry_runs;");
}

void DatabaseManager::rebuildVehicleSetupsIfNeeded()
{
    if(!tableExists("vehicle_setups") || !tableHasColumn("vehicle_setups", "vehicle_id"))
    {
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Rebuilding vehicle_setups");
    if(!execSql(
        "CREATE TABLE vehicle_setups_new ("
        "setup_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "run_id INTEGER NOT NULL UNIQUE REFERENCES telemetry_runs(run_id) ON DELETE CASCADE,"
        "wing_angle_front REAL NOT NULL DEFAULT 0,"
        "wing_angle_rear REAL NOT NULL DEFAULT 0,"
        "brake_bias REAL NOT NULL DEFAULT 50,"
        "suspension_stiffness REAL NOT NULL DEFAULT 0"
        ");"
    ))
    {
        return;
    }

    std::string insertSql =
        "INSERT INTO vehicle_setups_new (setup_id, run_id, wing_angle_front, wing_angle_rear, brake_bias, suspension_stiffness) "
        "SELECT vs.setup_id, "
        "(SELECT tr.run_id FROM telemetry_runs tr WHERE tr.vehicle_id = vs.vehicle_id ORDER BY tr.run_id DESC LIMIT 1), ";

    if(tableHasColumn("vehicle_setups", "front_wing_angle"))
    {
        insertSql += "COALESCE(vs.front_wing_angle, 0), ";
    }
    else
    {
        insertSql += "0, ";
    }

    if(tableHasColumn("vehicle_setups", "rear_wing_angle"))
    {
        insertSql += "COALESCE(vs.rear_wing_angle, 0), ";
    }
    else
    {
        insertSql += "0, ";
    }

    if(tableHasColumn("vehicle_setups", "brake_bias"))
    {
        insertSql += "COALESCE(vs.brake_bias, 50), ";
    }
    else
    {
        insertSql += "50, ";
    }

    insertSql +=
        "COALESCE(CAST(vs.suspension_stiffness AS REAL), 0) "
        "FROM vehicle_setups vs "
        "WHERE (SELECT tr.run_id FROM telemetry_runs tr WHERE tr.vehicle_id = vs.vehicle_id ORDER BY tr.run_id DESC LIMIT 1) IS NOT NULL;";

    if(!execSql(insertSql))
    {
        execSql("DROP TABLE IF EXISTS vehicle_setups_new;");
        return;
    }

    if(!execSql("DROP TABLE vehicle_setups;"))
    {
        return;
    }
    execSql("ALTER TABLE vehicle_setups_new RENAME TO vehicle_setups;");
}

void DatabaseManager::rebuildSensorReadingsIfNeeded()
{
    if(!tableExists("sensor_readings") || !tableHasColumn("sensor_readings", "recorded_at"))
    {
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Rebuilding sensor_readings");
    if(!execSql(
        "CREATE TABLE sensor_readings_new ("
        "reading_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "run_id INTEGER NOT NULL REFERENCES telemetry_runs(run_id) ON DELETE CASCADE,"
        "sensor_id INTEGER NOT NULL REFERENCES sensors(sensor_id),"
        "timestamp REAL NOT NULL,"
        "value REAL NOT NULL"
        ");"
    ))
    {
        return;
    }

    if(!execSql(
        "INSERT INTO sensor_readings_new (run_id, sensor_id, timestamp, value) "
        "SELECT run_id, sensor_id, "
        "CASE "
        "WHEN typeof(recorded_at) IN ('integer', 'real') THEN CAST(recorded_at AS REAL) "
        "WHEN recorded_at GLOB '[0-9]*' AND recorded_at NOT GLOB '*[^0-9.]*' THEN CAST(recorded_at AS REAL) "
        "ELSE CAST(rowid AS REAL) "
        "END, "
        "value "
        "FROM sensor_readings;"
    ))
    {
        execSql("DROP TABLE IF EXISTS sensor_readings_new;");
        return;
    }

    if(!execSql("DROP TABLE sensor_readings;"))
    {
        return;
    }
    execSql("ALTER TABLE sensor_readings_new RENAME TO sensor_readings;");
}

void DatabaseManager::migrateSchema()
{
    if(!needsLegacyMigration())
    {
        ensureIndexes();
        Logger::getInstance().log(INFO, "DBMAN: Schema already at ERD shape");
        return;
    }

    Logger::getInstance().log(INFO, "DBMAN: Migrating legacy schema to ERD");
    if(!execSql("PRAGMA foreign_keys = OFF;"))
    {
        return;
    }
    if(!execSql("BEGIN;"))
    {
        execSql("PRAGMA foreign_keys = ON;");
        return;
    }

    rebuildTracksIfNeeded();
    rebuildPartsInventoryIfNeeded();
    rebuildVehiclePartsIfNeeded();
    rebuildRaceSessionsIfNeeded();
    rebuildVehiclesIfNeeded();
    rebuildTelemetryRunsIfNeeded();
    rebuildVehicleSetupsIfNeeded();
    rebuildSensorReadingsIfNeeded();

    if(!execSql("COMMIT;"))
    {
        execSql("ROLLBACK;");
        Logger::getInstance().log(ERROR, "DBMAN: Legacy migration failed, rolled back");
    }
    else
    {
        Logger::getInstance().log(INFO, "DBMAN: Legacy migration completed");
    }

    execSql("PRAGMA foreign_keys = ON;");
    ensureIndexes();
}
