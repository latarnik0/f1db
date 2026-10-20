#include "Mocks/MockReadings.hpp"
#include <algorithm>
#include <string>
#include <vector>

std::vector<SensorReading> generateMockReadings(int sensorId, double minVal, double maxVal, int numberOfReadings)
{
    std::vector<SensorReading> readings;
    readings.reserve(numberOfReadings);
    for (int i=0; i<numberOfReadings; ++i)
    {
        double randomVal = minVal + (static_cast<double>(rand()) / RAND_MAX) * (maxVal - minVal);   
        readings.push_back({
            1, 
            sensorId, 
            "2026-10-10 12:00:00.000",
            randomVal
        });
    }
    return readings;
}

std::vector<std::vector<SensorReading>> prepareMockReadings()
{
    std::vector<std::vector<SensorReading>> result = {};
    result.reserve(4);
    auto pitotData = generateMockReadings(PITOT, 0.0, 370.0, 5000);
    result.push_back(pitotData);
    auto accelData = generateMockReadings(ACCEL, -6.0, 6.0, 5000);
    result.push_back(accelData);
    auto tensoData = generateMockReadings(TENSO, -20000.0, 20000.0, 5000);
    result.push_back(tensoData);
    auto irData = generateMockReadings(IR, 50.0, 150.0, 5000);
    result.push_back(irData);

    return result;
}