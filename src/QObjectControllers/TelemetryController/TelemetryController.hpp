#pragma once

#include "QueryHandler/QueryHandler.hpp"
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

class TelemetryController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList sensors READ sensors NOTIFY sensorsChanged)
    Q_PROPERTY(QVariantMap selectedRunDetails READ selectedRunDetails NOTIFY selectedRunDetailsChanged)
    Q_PROPERTY(QVariantList runEngineers READ runEngineers NOTIFY runEngineersChanged)
    Q_PROPERTY(QVariantList telemetryReadings READ telemetryReadings NOTIFY telemetryReadingsChanged)
    Q_PROPERTY(QVariantMap sensorStatistics READ sensorStatistics NOTIFY sensorStatisticsChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit TelemetryController(QueryHandler& queryHandler, QObject* parent = nullptr);

    QVariantList sensors() const;
    QVariantMap selectedRunDetails() const;
    QVariantList runEngineers() const;
    QVariantList telemetryReadings() const;
    QVariantMap sensorStatistics() const;
    QString statusMessage() const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool addSensor(const QString& name, const QString& unit);
    Q_INVOKABLE bool startTelemetryRun(int sessionId, int vehicleId, int driverId);
    Q_INVOKABLE bool setRunSectors(int runId, double sector1S, double sector2S, double sector3S, double lapTimeS);
    Q_INVOKABLE bool assignEngineerToRun(int runId, int engineerId);
    Q_INVOKABLE bool insertMockReadings(int runId);
    Q_INVOKABLE void loadRunDetails(int runId);
    Q_INVOKABLE void loadEngineersForRun(int runId);
    Q_INVOKABLE void loadRunTelemetry(int runId, int sensorId);
    Q_INVOKABLE QVariantList fetchRunTelemetry(int runId, int sensorId);
    Q_INVOKABLE void loadSensorStatistics(int runId, int sensorId);

signals:
    void sensorsChanged();
    void selectedRunDetailsChanged();
    void runEngineersChanged();
    void telemetryReadingsChanged();
    void sensorStatisticsChanged();
    void statusMessageChanged();

private:
    void setStatusMessage(const QString& message);
    void setSensors(const std::vector<Sensor>& sensors);
    void setSelectedRunDetails(const QVariantMap& details);
    void setRunEngineers(const std::vector<Engineer>& engineers);
    void setTelemetryReadings(const std::vector<SensorReading>& readings);
    void setSensorStatistics(const QVariantMap& statistics);

    QueryHandler& m_queryHandler;
    QVariantList m_sensors;
    QVariantList m_runEngineers;
    QVariantList m_telemetryReadings;
    QVariantMap m_selectedRunDetails;
    QVariantMap m_sensorStatistics;
    QString m_statusMessage;
};
