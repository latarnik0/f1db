#include "TelemetryController.hpp"
#include "Mocks/MockReadings.hpp"
#include <QVariantMap>

TelemetryController::TelemetryController(QueryHandler& queryHandler, QObject* parent)
    : QObject(parent)
    , m_queryHandler(queryHandler)
{
    refresh();
}

QVariantList TelemetryController::sensors() const
{
    return m_sensors;
}

QVariantMap TelemetryController::selectedRunDetails() const
{
    return m_selectedRunDetails;
}

QVariantList TelemetryController::runEngineers() const
{
    return m_runEngineers;
}

QVariantList TelemetryController::telemetryReadings() const
{
    return m_telemetryReadings;
}

QVariantMap TelemetryController::sensorStatistics() const
{
    return m_sensorStatistics;
}

QString TelemetryController::statusMessage() const
{
    return m_statusMessage;
}

void TelemetryController::refresh()
{
    setSensors(m_queryHandler.getAllSensors());
    setSelectedRunDetails({});
    setRunEngineers({});
    setTelemetryReadings({});
    setSensorStatistics({});
    setStatusMessage(QStringLiteral("Loaded %1 sensors").arg(m_sensors.size()));
}

bool TelemetryController::addSensor(const QString& name, const QString& unit)
{
    if(name.trimmed().isEmpty() || unit.trimmed().isEmpty())
    {
        setStatusMessage(QStringLiteral("Name and unit required"));
        return false;
    }

    const int newId = m_queryHandler.addSensor(
        name.trimmed().toStdString(),
        unit.trimmed().toStdString());

    if(newId < 0)
    {
        setStatusMessage(QStringLiteral("Failed to add sensor"));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Added sensor #%1").arg(newId));
    return true;
}

bool TelemetryController::startTelemetryRun(int sessionId, int vehicleId, int driverId)
{
    if(sessionId <= 0 || vehicleId <= 0 || driverId <= 0)
    {
        setStatusMessage(QStringLiteral("Session, vehicle and driver ids must be > 0"));
        return false;
    }

    const int newId = m_queryHandler.startTelemetryRun(sessionId, vehicleId, driverId);

    if(newId < 0)
    {
        setStatusMessage(QStringLiteral("Failed to start telemetry run"));
        return false;
    }
    setStatusMessage(QStringLiteral("Started telemetry run #%1").arg(newId));
    return true;
}

bool TelemetryController::setRunSectors(int runId, double sector1S, double sector2S, double sector3S, double lapTimeS)
{
    if(runId <= 0)
    {
        setStatusMessage(QStringLiteral("Run id must be > 0"));
        return false;
    }

    if(!m_queryHandler.setRunSectors(runId, sector1S, sector2S, sector3S, lapTimeS))
    {
        setStatusMessage(QStringLiteral("Failed to set sectors for run #%1").arg(runId));
        return false;
    }

    if(m_selectedRunDetails.value(QStringLiteral("runId")).toInt() == runId)
    {
        loadRunDetails(runId);
    }
    setStatusMessage(QStringLiteral("Updated sectors for run #%1").arg(runId));
    return true;
}

bool TelemetryController::assignEngineerToRun(int runId, int engineerId)
{
    if(runId <= 0 || engineerId <= 0)
    {
        setStatusMessage(QStringLiteral("Run and engineer ids must be > 0"));
        return false;
    }

    if(!m_queryHandler.assignEngineerToRun(runId, engineerId))
    {
        setStatusMessage(QStringLiteral("Failed to assign engineer #%1 to run #%2")
                             .arg(engineerId)
                             .arg(runId));
        return false;
    }

    if(m_selectedRunDetails.value(QStringLiteral("runId")).toInt() == runId)
    {
        loadEngineersForRun(runId);
    }
    setStatusMessage(QStringLiteral("Assigned engineer #%1 to run #%2")
                         .arg(engineerId)
                         .arg(runId));
    return true;
}

bool TelemetryController::insertMockReadings(int runId)
{
    if(runId <= 0)
    {
        setStatusMessage(QStringLiteral("Run id must be > 0"));
        return false;
    }

    MockReadings mock;
    auto data = mock.prepareMockReadings();
    for(auto& sensorVec : data)
    {
        for(auto& reading : sensorVec)
        {
            reading.runId = runId;
        }
    }
    m_queryHandler.insertMockReadings(data);
    setStatusMessage(QStringLiteral("Inserted mock readings for run #%1").arg(runId));
    return true;
}

void TelemetryController::loadRunDetails(int runId)
{
    const RunDetails details = m_queryHandler.getRunDetails(runId);

    if(details.runId <= 0)
    {
        setSelectedRunDetails({});
        setStatusMessage(QStringLiteral("Failed to load run #%1").arg(runId));
        return;
    }

    setSelectedRunDetails(QVariantMap{
        {QStringLiteral("runId"), details.runId},
        {QStringLiteral("sessionDate"), QString::fromStdString(details.sessionDate)},
        {QStringLiteral("chassisCode"), QString::fromStdString(details.chassisCode)},
        {QStringLiteral("driverFirstName"), QString::fromStdString(details.driverFirstName)},
        {QStringLiteral("driverLastName"), QString::fromStdString(details.driverLastName)},
        {QStringLiteral("sector1S"), details.sector1S},
        {QStringLiteral("sector2S"), details.sector2S},
        {QStringLiteral("sector3S"), details.sector3S},
        {QStringLiteral("lapTimeS"), details.lapTimeS},
    });
    setStatusMessage(QStringLiteral("Loaded run #%1 details").arg(details.runId));
}

void TelemetryController::loadEngineersForRun(int runId)
{
    setRunEngineers(m_queryHandler.getEngineersForRun(runId));
    setStatusMessage(QStringLiteral("Loaded %1 engineers for run #%2")
                         .arg(m_runEngineers.size())
                         .arg(runId));
}

void TelemetryController::loadRunTelemetry(int runId, int sensorId)
{
    setTelemetryReadings(m_queryHandler.getRunTelemetry(runId, sensorId));
    setStatusMessage(QStringLiteral("Loaded %1 readings for run #%2 sensor #%3")
                         .arg(m_telemetryReadings.size())
                         .arg(runId)
                         .arg(sensorId));
}

QVariantList TelemetryController::fetchRunTelemetry(int runId, int sensorId)
{
    QVariantList result;
    const std::vector<SensorReading> readings = m_queryHandler.getRunTelemetry(runId, sensorId);
    result.reserve(static_cast<int>(readings.size()));

    for(const SensorReading& reading : readings)
    {
        result.append(QVariantMap{
            {QStringLiteral("runId"), reading.runId},
            {QStringLiteral("sensorId"), reading.sensorId},
            {QStringLiteral("recordedAt"), QString::fromStdString(reading.recordedAt)},
            {QStringLiteral("value"), reading.value},
        });
    }
    return result;
}

void TelemetryController::loadSensorStatistics(int runId, int sensorId)
{
    const SensorStatistics stats = m_queryHandler.getSensorStatistics(runId, sensorId);

    if(stats.runId <= 0)
    {
        setSensorStatistics({});
        setStatusMessage(QStringLiteral("Failed to load statistics for run #%1 sensor #%2")
                             .arg(runId)
                             .arg(sensorId));
        return;
    }

    setSensorStatistics(QVariantMap{
        {QStringLiteral("runId"), stats.runId},
        {QStringLiteral("sensorId"), stats.sensorId},
        {QStringLiteral("maxValue"), stats.maxValue},
        {QStringLiteral("minValue"), stats.minValue},
        {QStringLiteral("avgValue"), stats.avgValue},
    });
    setStatusMessage(QStringLiteral("Loaded statistics for run #%1 sensor #%2")
                         .arg(stats.runId)
                         .arg(stats.sensorId));
}

void TelemetryController::setStatusMessage(const QString& message)
{
    if(m_statusMessage == message)
    {
        return;
    }
    m_statusMessage = message;
    emit statusMessageChanged();
}

void TelemetryController::setSensors(const std::vector<Sensor>& sensors)
{
    m_sensors.clear();
    m_sensors.reserve(static_cast<int>(sensors.size()));

    for(const Sensor& sensor : sensors)
    {
        m_sensors.append(QVariantMap{
            {QStringLiteral("id"), sensor.id},
            {QStringLiteral("name"), QString::fromStdString(sensor.name)},
            {QStringLiteral("unit"), QString::fromStdString(sensor.unit)},
        });
    }
    emit sensorsChanged();
}

void TelemetryController::setSelectedRunDetails(const QVariantMap& details)
{
    if(m_selectedRunDetails == details)
    {
        return;
    }
    m_selectedRunDetails = details;
    emit selectedRunDetailsChanged();
}

void TelemetryController::setRunEngineers(const std::vector<Engineer>& engineers)
{
    m_runEngineers.clear();
    m_runEngineers.reserve(static_cast<int>(engineers.size()));

    for(const Engineer& engineer : engineers)
    {
        m_runEngineers.append(QVariantMap{
            {QStringLiteral("id"), engineer.id},
            {QStringLiteral("firstName"), QString::fromStdString(engineer.firstName)},
            {QStringLiteral("lastName"), QString::fromStdString(engineer.lastName)},
            {QStringLiteral("division"), QString::fromStdString(engineer.division)},
        });
    }
    emit runEngineersChanged();
}

void TelemetryController::setTelemetryReadings(const std::vector<SensorReading>& readings)
{
    m_telemetryReadings.clear();
    m_telemetryReadings.reserve(static_cast<int>(readings.size()));

    for(const SensorReading& reading : readings)
    {
        m_telemetryReadings.append(QVariantMap{
            {QStringLiteral("runId"), reading.runId},
            {QStringLiteral("sensorId"), reading.sensorId},
            {QStringLiteral("recordedAt"), QString::fromStdString(reading.recordedAt)},
            {QStringLiteral("value"), reading.value},
        });
    }
    emit telemetryReadingsChanged();
}

void TelemetryController::setSensorStatistics(const QVariantMap& statistics)
{
    if(m_sensorStatistics == statistics)
    {
        return;
    }
    m_sensorStatistics = statistics;
    emit sensorStatisticsChanged();
}
