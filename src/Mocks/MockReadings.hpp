#pragma once
#include <algorithm>
#include <string>
#include <vector>

enum SENSOR
{
    PITOT = 1,
    ACCEL = 2,
    TENSO = 3,
    IR = 4
};

struct SensorReading
{
    int runId;
    int sensorId;
    std::string recordedAt;
    double value;
};

class MockReadings
{
public:
    MockReadings();
    ~MockReadings();
    
    SENSOR s;
    SensorReading sr;

    std::vector<SensorReading> generateMockReadings(int sensorId, double minVal, double maxVal, int numberOfReadings = 5000);
    std::vector<std::vector<SensorReading>> prepareMockReadings();
};
