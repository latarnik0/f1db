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
    std::vector<TelemetryRun> getAllRuns();
    int startTelemetryRun(int sessionId, int vehicleId, int driverId);
    bool setRunSectors(int runId, double sector1S, double sector2S, double sector3S, double lapTimeS);
    bool assignEngineerToRun(int runId, int engineerId);
    void clearReadingsForRun(int runId);
    void insertBatch(std::vector<std::vector<MockSensorReading>>& sensorData);
    RunDetails getRunDetails(int runId);
    std::vector<Engineer> getEngineersForRun(int runId);
    std::vector<SensorReading> getRunTelemetry(int runId, int sensorId);
    SensorStatistics getSensorStatistics(int runId, int sensorId);
};