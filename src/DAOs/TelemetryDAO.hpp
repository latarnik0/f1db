#pragma once
#include <sqlite3.h>
#include "Mocks/MockReadings.hpp"
#include "Models/Models.hpp"

class TelemetryDAO
{
private:
    sqlite3* db;

public:
    TelemetryDAO(sqlite3* db_connection);
    ~TelemetryDAO();

    int addSensor(const std::string& name, const std::string& unit);
    std::vector<Sensor> getAllSensors();
    int startTelemetryRun(int sessionId, int vehicleId, int driverId);
    bool assignEngineerToRun(int runId, int engineerId);
    void insertBatch(std::vector<std::vector<SensorReading>>& sensorData);
    // analytical data
    RunDetails getRunDetails(int runId);
    std::vector<Engineer> getEngineersForRun(int runId);
    std::vector<SensorReading> getRunTelemetry(int runId, int sensorId);
    SensorStatistics getSensorStatistics(int runId, int sensorId);
};