#pragma once
#include "Logger/Logger.hpp"
#include <sqlite3.h>
#include <string>

class DatabaseManager
{
private:
    sqlite3* db;

    void ensureSchema();
    void migrateSchema();
    void ensureIndexes();
    bool tableExists(const std::string& tableName);
    bool tableHasColumn(const std::string& tableName, const std::string& columnName);
    void addColumnIfMissing(const std::string& tableName, const std::string& columnName, const std::string& columnDef);
    bool execSql(const std::string& sql);
    bool needsLegacyMigration();
    void ensurePlaceholderDriverIfNeeded();
    void rebuildTracksIfNeeded();
    void rebuildPartsInventoryIfNeeded();
    void rebuildVehiclePartsIfNeeded();
    void rebuildRaceSessionsIfNeeded();
    void rebuildVehiclesIfNeeded();
    void rebuildTelemetryRunsIfNeeded();
    void rebuildVehicleSetupsIfNeeded();
    void rebuildSensorReadingsIfNeeded();

public:
    DatabaseManager(const char* db_path);
    ~DatabaseManager();
    sqlite3* getConnection();
    bool isConnected() const;
};
