#pragma once
#include <string>

// ==========================================
// Personnel, dictionaries
// ==========================================

struct Track
{
    int id;
    std::string name;
    std::string location;
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
    int driverId;
};

struct Part
{
    int id;
    std::string name;
    std::string category;
    int stockQuantity;
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
    int runId;
    double frontWingAngle;
    double rearWingAngle;
    double brakeBias;
    double suspensionStiffness;
};

struct SetupParamDelta
{
    double valueA;
    double valueB;
    double delta;
};

struct SetupComparison
{
    int runIdA;
    int runIdB;
    bool validA;
    bool validB;
    SetupParamDelta frontWingAngle;
    SetupParamDelta rearWingAngle;
    SetupParamDelta brakeBias;
    SetupParamDelta suspensionStiffness;
};

struct InstalledPart
{
    int partId;
    std::string name;
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
    std::string sessionType;
    std::string sessionDate;
    std::string weather;
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
    std::string chassisCode;
    std::string driverName;
};

struct RunEngineer
{
    int runId;
    int engineerId;
};

struct SensorReading
{
    int readingId;
    int runId;
    int sensorId;
    double timestamp;
    double value;
};

struct SessionDetails
{
    int sessionId;
    std::string sessionDate;
    int trackId;
    std::string trackName;
    std::string trackLocation;
    std::string sessionType;
    std::string weather;
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
