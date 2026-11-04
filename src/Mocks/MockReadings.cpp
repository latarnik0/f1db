#include "Mocks/MockReadings.hpp"
#include "Logger/Logger.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace
{
double clampValue(double value, double minVal, double maxVal)
{
    if(value < minVal)
    {
        return minVal;
    }
    if(value > maxVal)
    {
        return maxVal;
    }
    return value;
}

double unitNoise()
{
    return (static_cast<double>(rand()) / RAND_MAX) * 2.0 - 1.0;
}

double smoothPulse(double t, double center, double halfWidth)
{
    const double x = (t - center) / halfWidth;
    if(x <= -1.0 || x >= 1.0)
    {
        return 0.0;
    }
    return 0.5 * (1.0 + std::cos(std::acos(-1.0) * x));
}

double lapSpeedProfile(double t)
{
    double speed = 0.22
                   + 0.55 * (1.0 - std::exp(-8.0 * t))
                   - 0.38 * smoothPulse(t, 0.22, 0.05)
                   - 0.28 * smoothPulse(t, 0.45, 0.04)
                   - 0.42 * smoothPulse(t, 0.68, 0.06)
                   - 0.20 * smoothPulse(t, 0.88, 0.04);
    if(t > 0.92)
    {
        speed -= 0.35 * ((t - 0.92) / 0.08);
    }
    return clampValue(speed, 0.05, 0.95);
}

std::string lowerCopy(std::string text)
{
    std::transform(text.begin(), text.end(), text.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return text;
}

bool nameHas(const std::string& lowerName, const char* token)
{
    return lowerName.find(token) != std::string::npos;
}

std::string formatTimestamp(int sampleIndex)
{
    const int totalMs = sampleIndex * 10;
    const int totalSec = totalMs / 1000;
    const int ms = totalMs % 1000;
    const int minutes = (totalSec / 60) % 60;
    const int seconds = totalSec % 60;
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "2026-10-10 12:%02d:%02d.%03d", minutes, seconds, ms);
    return buffer;
}

void rangesForWaveform(SENSOR waveform, double& minVal, double& maxVal)
{
    switch(waveform)
    {
    case PITOT:
        minVal = 40.0;
        maxVal = 340.0;
        break;
    case ACCEL:
        minVal = -6.0;
        maxVal = 6.0;
        break;
    case TENSO:
        minVal = -20000.0;
        maxVal = 20000.0;
        break;
    case IR:
        minVal = 55.0;
        maxVal = 145.0;
        break;
    default:
        minVal = 0.0;
        maxVal = 1.0;
        break;
    }
}
}

MockReadings::MockReadings() {}
MockReadings::~MockReadings() {}

SENSOR MockReadings::waveformFromName(const std::string& name)
{
    const std::string lower = lowerCopy(name);
    if(nameHas(lower, "pitot") || nameHas(lower, "airspeed") || nameHas(lower, "speed"))
    {
        return PITOT;
    }
    if(nameHas(lower, "acl") || nameHas(lower, "accel") || nameHas(lower, "g-force") || nameHas(lower, "imu"))
    {
        return ACCEL;
    }
    if(nameHas(lower, "tenso") || nameHas(lower, "strain") || nameHas(lower, "load"))
    {
        return TENSO;
    }
    if(nameHas(lower, "ir") || nameHas(lower, "temp") || nameHas(lower, "thermal"))
    {
        return IR;
    }
    return PITOT;
}

std::vector<MockSensorReading> MockReadings::generateMockReadings(int dbSensorId,
                                                                  SENSOR waveform,
                                                                  double minVal,
                                                                  double maxVal,
                                                                  int numberOfReadings)
{
    std::vector<MockSensorReading> readings;
    readings.reserve(numberOfReadings);

    const double pi = std::acos(-1.0);
    const double span = maxVal - minVal;

    for(int i = 0; i < numberOfReadings; ++i)
    {
        const double t = static_cast<double>(i) / static_cast<double>(numberOfReadings);
        const double speedNorm = lapSpeedProfile(t);
        const double brake1 = smoothPulse(t, 0.22, 0.035);
        const double brake2 = smoothPulse(t, 0.45, 0.03);
        const double brake3 = smoothPulse(t, 0.68, 0.045);
        const double brake4 = smoothPulse(t, 0.88, 0.03);
        const double braking = brake1 + brake2 + brake3 + brake4;
        const double cornerLoad = smoothPulse(t, 0.24, 0.055)
                                  + smoothPulse(t, 0.47, 0.05)
                                  + smoothPulse(t, 0.71, 0.07)
                                  + smoothPulse(t, 0.90, 0.045);
        double value = 0.0;

        switch(waveform)
        {
        case PITOT:
        {
            const double base = minVal + speedNorm * span;
            const double wobble = 0.012 * span * std::sin(2.0 * pi * 14.0 * t);
            value = base + wobble + 0.008 * span * unitNoise();
            break;
        }
        case ACCEL:
        {
            const double longG = -4.2 * braking
                                 + 1.6 * (1.0 - braking) * (0.35 + 0.15 * std::sin(2.0 * pi * 2.0 * t));
            const double latG = 3.8 * cornerLoad * std::sin(2.0 * pi * 1.0 * t + 0.7);
            const double chatter = braking > 0.05
                                       ? 0.55 * std::sin(2.0 * pi * 42.0 * t) * braking
                                       : 0.08 * std::sin(2.0 * pi * 9.0 * t);
            value = longG + latG + chatter + 0.12 * unitNoise();
            break;
        }
        case TENSO:
        {
            const double meanLoad = (0.12 + 0.78 * cornerLoad) * span * 0.45;
            const double side = (cornerLoad > 0.15 ? 1.0 : -0.15)
                                * meanLoad
                                * (0.55 + 0.45 * std::tanh(12.0 * (cornerLoad - 0.2)));
            const double settle = 0.04 * span * std::sin(2.0 * pi * 0.7 * t);
            value = side + settle + 0.015 * span * unitNoise();
            break;
        }
        case IR:
        {
            const double heatFromSpeed = 0.55 * speedNorm;
            const double heatFromBrake = 0.35 * braking;
            const double thermal = 0.18 + 0.55 * t + 0.22 * heatFromSpeed + 0.25 * heatFromBrake;
            const double coolStraight = -0.04 * (1.0 - braking) * (1.0 - cornerLoad);
            value = minVal + clampValue(thermal + coolStraight, 0.0, 1.0) * span
                    + 0.006 * span * unitNoise();
            break;
        }
        default:
            value = minVal + 0.5 * span + 0.1 * span * std::sin(2.0 * pi * 2.0 * t);
            break;
        }

        readings.push_back({
            1,
            dbSensorId,
            formatTimestamp(i),
            clampValue(value, minVal, maxVal)
        });
    }
    return readings;
}

std::vector<std::vector<MockSensorReading>> MockReadings::prepareMockReadings(const std::vector<Sensor>& sensors)
{
    std::vector<std::vector<MockSensorReading>> result;
    result.reserve(sensors.size());

    for(const Sensor& sensor : sensors)
    {
        const SENSOR waveform = waveformFromName(sensor.name);
        double minVal = 0.0;
        double maxVal = 1.0;
        rangesForWaveform(waveform, minVal, maxVal);
        result.push_back(generateMockReadings(sensor.id, waveform, minVal, maxVal, 2000));
    }

    Logger::getInstance().log(INFO, "MOCK: Mock readings generated for registered sensors");
    return result;
}
