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
    double aeroBalance;
    std::string suspensionStiffness;
};

struct InstalledPart
{
    int partId;
    std::string partName;
    std::string category;
    std::string installedAt;
};

// ==========================================
// Telemetry
// ==========================================

struct RaceSession
{
    int id;
    int trackId;
    std::string sessionDate;
};

struct TelemetryRun
{
    int id;
    int sessionId;
    int vehicleId;
    int driverId;
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
};