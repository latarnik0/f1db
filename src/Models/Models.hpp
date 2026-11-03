#pragma once
#include <string>

// ==========================================
// Personnel, dictionaries
// ==========================================

struct Track
{
    int id;
    std::string name;
    double lengthKm;
};

struct Driver
{
    int id;
    std::string firstName;
    std::string lastName;
    int experienceLvl;
};

struct Engineer
{
    int id;
    std::string firstName;
    std::string lastName;
    std::string division;
};

struct Sensor
{
    int id;
    std::string name;
    std::string unit;
};

// ==========================================
// Vehicles and parts
// ==========================================

struct Vehicle
{
    int id;
    std::string chassisCode;
};

struct Part
{
    int id;
    std::string partName;
    std::string category;
    double wearPercent;
};

struct VehiclePart
{
    int vehicleId;
    int partId;
    std::string installedAt;
};

struct VehicleSetup
{
    int id;
    int vehicleId;
    double frontWingAngle;
    double rearWingAngle;
    double brakeBias;
    std::string suspensionStiffness;
};

struct InstalledPart
{
    int partId;
    std::string partName;
    std::string category;
    std::string installedAt;
    double wearPercent;
};

// ==========================================
// Telemetry
// ==========================================

struct RaceSession
{
    int id;
    int trackId;
    std::string sessionDate;
    std::string weatherCondition;
    double temperatureC;
};

struct TelemetryRun
{
    int id;
    int sessionId;
    int vehicleId;
    int driverId;
    double sector1S;
    double sector2S;
    double sector3S;
    double lapTimeS;
};

struct RunEngineer
{
    int runId;
    int engineerId;
};

struct SensorReading
{
    int runId;
    int sensorId;
    std::string recordedAt;
    double value;
};

struct SessionDetails
{
    int sessionId;
    std::string sessionDate;
    int trackId;
    std::string trackName;
    double trackLengthKm;
    std::string weatherCondition;
    double temperatureC;
};

struct RunDetails
{
    int runId;
    std::string sessionDate;
    std::string chassisCode;
    std::string driverFirstName;
    std::string driverLastName;
    double sector1S;
    double sector2S;
    double sector3S;
    double lapTimeS;
};

struct SensorStatistics
{
    int runId;
    int sensorId;
    double maxValue;
    double minValue;
    double avgValue;
};
