#pragma once

#include <QObject>
#include <QString>

class AppState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool dbConnected READ dbConnected NOTIFY dbConnectedChanged)
    Q_PROPERTY(QString dbPath READ dbPath NOTIFY dbPathChanged)
    Q_PROPERTY(int selectedDriverId READ selectedDriverId WRITE setSelectedDriverId NOTIFY selectionChanged)
    Q_PROPERTY(int selectedTrackId READ selectedTrackId WRITE setSelectedTrackId NOTIFY selectionChanged)
    Q_PROPERTY(int selectedSessionId READ selectedSessionId WRITE setSelectedSessionId NOTIFY selectionChanged)
    Q_PROPERTY(int selectedVehicleId READ selectedVehicleId WRITE setSelectedVehicleId NOTIFY selectionChanged)
    Q_PROPERTY(int selectedRunId READ selectedRunId WRITE setSelectedRunId NOTIFY selectionChanged)
    Q_PROPERTY(QString driverLabel READ driverLabel WRITE setDriverLabel NOTIFY selectionChanged)
    Q_PROPERTY(QString trackLabel READ trackLabel WRITE setTrackLabel NOTIFY selectionChanged)
    Q_PROPERTY(QString sessionLabel READ sessionLabel WRITE setSessionLabel NOTIFY selectionChanged)
    Q_PROPERTY(QString vehicleLabel READ vehicleLabel WRITE setVehicleLabel NOTIFY selectionChanged)

public:
    explicit AppState(QObject* parent = nullptr);

    bool dbConnected() const;
    QString dbPath() const;
    int selectedDriverId() const;
    int selectedTrackId() const;
    int selectedSessionId() const;
    int selectedVehicleId() const;
    int selectedRunId() const;
    QString driverLabel() const;
    QString trackLabel() const;
    QString sessionLabel() const;
    QString vehicleLabel() const;

    void setDbConnected(bool connected);
    void setDbPath(const QString& path);
    void setSelectedDriverId(int id);
    void setSelectedTrackId(int id);
    void setSelectedSessionId(int id);
    void setSelectedVehicleId(int id);
    void setSelectedRunId(int id);
    void setDriverLabel(const QString& label);
    void setTrackLabel(const QString& label);
    void setSessionLabel(const QString& label);
    void setVehicleLabel(const QString& label);

signals:
    void dbConnectedChanged();
    void dbPathChanged();
    void selectionChanged();

private:
    bool m_dbConnected = false;
    QString m_dbPath;
    int m_selectedDriverId = 0;
    int m_selectedTrackId = 0;
    int m_selectedSessionId = 0;
    int m_selectedVehicleId = 0;
    int m_selectedRunId = 0;
    QString m_driverLabel = "—";
    QString m_trackLabel = "—";
    QString m_sessionLabel = "—";
    QString m_vehicleLabel = "—";
};
