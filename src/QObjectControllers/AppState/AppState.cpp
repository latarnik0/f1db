#include "AppState.hpp"

AppState::AppState(QObject* parent)
    : QObject(parent)
{
}

bool AppState::dbConnected() const
{
    return m_dbConnected;
}

QString AppState::dbPath() const
{
    return m_dbPath;
}

int AppState::selectedDriverId() const
{
    return m_selectedDriverId;
}

int AppState::selectedTrackId() const
{
    return m_selectedTrackId;
}

int AppState::selectedSessionId() const
{
    return m_selectedSessionId;
}

int AppState::selectedVehicleId() const
{
    return m_selectedVehicleId;
}

int AppState::selectedRunId() const
{
    return m_selectedRunId;
}

QString AppState::driverLabel() const
{
    return m_driverLabel;
}

QString AppState::trackLabel() const
{
    return m_trackLabel;
}

QString AppState::sessionLabel() const
{
    return m_sessionLabel;
}

QString AppState::vehicleLabel() const
{
    return m_vehicleLabel;
}

void AppState::setDbConnected(bool connected)
{
    if(m_dbConnected == connected)
    {
        return;
    }
    m_dbConnected = connected;
    emit dbConnectedChanged();
}

void AppState::setDbPath(const QString& path)
{
    if(m_dbPath == path)
    {
        return;
    }
    m_dbPath = path;
    emit dbPathChanged();
}

void AppState::setSelectedDriverId(int id)
{
    if(m_selectedDriverId == id)
    {
        return;
    }
    m_selectedDriverId = id;
    emit selectionChanged();
}

void AppState::setSelectedTrackId(int id)
{
    if(m_selectedTrackId == id)
    {
        return;
    }
    m_selectedTrackId = id;
    emit selectionChanged();
}

void AppState::setSelectedSessionId(int id)
{
    if(m_selectedSessionId == id)
    {
        return;
    }
    m_selectedSessionId = id;
    emit selectionChanged();
}

void AppState::setSelectedVehicleId(int id)
{
    if(m_selectedVehicleId == id)
    {
        return;
    }
    m_selectedVehicleId = id;
    emit selectionChanged();
}

void AppState::setSelectedRunId(int id)
{
    if(m_selectedRunId == id)
    {
        return;
    }
    m_selectedRunId = id;
    emit selectionChanged();
}

void AppState::setDriverLabel(const QString& label)
{
    if(m_driverLabel == label)
    {
        return;
    }
    m_driverLabel = label;
    emit selectionChanged();
}

void AppState::setTrackLabel(const QString& label)
{
    if(m_trackLabel == label)
    {
        return;
    }
    m_trackLabel = label;
    emit selectionChanged();
}

void AppState::setSessionLabel(const QString& label)
{
    if(m_sessionLabel == label)
    {
        return;
    }
    m_sessionLabel = label;
    emit selectionChanged();
}

void AppState::setVehicleLabel(const QString& label)
{
    if(m_vehicleLabel == label)
    {
        return;
    }
    m_vehicleLabel = label;
    emit selectionChanged();
}
