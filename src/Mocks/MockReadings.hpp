#pragma once
#include <string>
#include <vector>
#include "Models/Models.hpp"

enum SENSOR
{
    PITOT = 1,
    ACCEL = 2,
    TENSO = 3,
    IR = 4
};

struct MockSensorReading
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

    static SENSOR waveformFromName(const std::string& name);
    std::vector<MockSensorReading> generateMockReadings(int dbSensorId,
                                                        SENSOR waveform,
                                                        double minVal,
                                                        double maxVal,
                                                        int numberOfReadings = 2000);
    std::vector<std::vector<MockSensorReading>> prepareMockReadings(const std::vector<Sensor>& sensors);
};
