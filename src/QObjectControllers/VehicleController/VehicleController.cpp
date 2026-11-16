#include "VehicleController.hpp"
#include <QVariantMap>

VehicleController::VehicleController(QueryHandler& queryHandler, QObject* parent)
    : QObject(parent)
    , m_queryHandler(queryHandler)
{
    refresh();
}

QVariantList VehicleController::vehicles() const
{
    return m_vehicles;
}

QVariantMap VehicleController::selectedSetup() const
{
    return m_selectedSetup;
}

QVariantList VehicleController::installedParts() const
{
    return m_installedParts;
}

QString VehicleController::statusMessage() const
{
    return m_statusMessage;
}

void VehicleController::refresh()
{
    setVehicles(m_queryHandler.getAllVehicles());
    setSelectedSetup({});
    setInstalledParts({});
    setStatusMessage(QStringLiteral("Loaded %1 vehicles").arg(m_vehicles.size()));
}

bool VehicleController::addVehicle(const QString& chassisCode, int driverId)
{
    if(chassisCode.trimmed().isEmpty())
    {
        setStatusMessage(QStringLiteral("Chassis code required"));
        return false;
    }

    if(driverId <= 0)
    {
        setStatusMessage(QStringLiteral("Driver id must be > 0"));
        return false;
    }

    const int newId = m_queryHandler.addVehicle(chassisCode.trimmed().toStdString(), driverId);

    if(newId < 0)
    {
        setStatusMessage(QStringLiteral("Failed to add vehicle"));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Added vehicle #%1").arg(newId));
    return true;
}

bool VehicleController::removeVehicle(int vehicleId)
{
    if(!m_queryHandler.deleteVehicle(vehicleId))
    {
        setStatusMessage(QStringLiteral("Failed to delete vehicle #%1").arg(vehicleId));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Deleted vehicle #%1").arg(vehicleId));
    return true;
}

bool VehicleController::setRunSetup(int runId, double frontWingAngle, double rearWingAngle, double brakeBias, double suspensionStiffness)
{
    if(runId <= 0)
    {
        setStatusMessage(QStringLiteral("Run id must be > 0"));
        return false;
    }

    if(!m_queryHandler.setRunSetup(
            runId,
            frontWingAngle,
            rearWingAngle,
            brakeBias,
            suspensionStiffness))
    {
        setStatusMessage(QStringLiteral("Failed to set setup for run #%1").arg(runId));
        return false;
    }

    loadSetup(runId);
    setStatusMessage(QStringLiteral("Updated setup for run #%1").arg(runId));
    return true;
}

void VehicleController::loadSetup(int runId)
{
    const VehicleSetup setup = m_queryHandler.getRunSetup(runId);

    if(setup.id <= 0 || setup.runId <= 0)
    {
        setSelectedSetup({});
        setStatusMessage(QStringLiteral("Failed to load setup for run #%1").arg(runId));
        return;
    }

    setSelectedSetup(QVariantMap{
        {QStringLiteral("id"), setup.id},
        {QStringLiteral("runId"), setup.runId},
        {QStringLiteral("frontWingAngle"), setup.frontWingAngle},
        {QStringLiteral("rearWingAngle"), setup.rearWingAngle},
        {QStringLiteral("brakeBias"), setup.brakeBias},
        {QStringLiteral("suspensionStiffness"), setup.suspensionStiffness},
    });
    setStatusMessage(QStringLiteral("Loaded setup for run #%1").arg(setup.runId));
}

bool VehicleController::installPart(int vehicleId, int partId)
{
    if(vehicleId <= 0 || partId <= 0)
    {
        setStatusMessage(QStringLiteral("Vehicle and part ids must be > 0"));
        return false;
    }

    if(!m_queryHandler.installPartOnVehicle(vehicleId, partId))
    {
        setStatusMessage(QStringLiteral("Failed to install part #%1 on vehicle #%2")
                             .arg(partId)
                             .arg(vehicleId));
        return false;
    }

    loadInstalledParts(vehicleId);
    setStatusMessage(QStringLiteral("Installed part #%1 on vehicle #%2")
                         .arg(partId)
                         .arg(vehicleId));
    return true;
}

bool VehicleController::removePart(int vehicleId, int partId)
{
    if(vehicleId <= 0 || partId <= 0)
    {
        setStatusMessage(QStringLiteral("Vehicle and part ids must be > 0"));
        return false;
    }

    if(!m_queryHandler.removePartFromVehicle(vehicleId, partId))
    {
        setStatusMessage(QStringLiteral("Failed to remove part #%1 from vehicle #%2")
                             .arg(partId)
                             .arg(vehicleId));
        return false;
    }

    loadInstalledParts(vehicleId);
    setStatusMessage(QStringLiteral("Removed part #%1 from vehicle #%2")
                         .arg(partId)
                         .arg(vehicleId));
    return true;
}

void VehicleController::loadInstalledParts(int vehicleId)
{
    setInstalledParts(m_queryHandler.getVehicleWithInstalledParts(vehicleId));
    setStatusMessage(QStringLiteral("Loaded %1 installed parts for vehicle #%2")
                         .arg(m_installedParts.size())
                         .arg(vehicleId));
}

void VehicleController::setStatusMessage(const QString& message)
{
    if(m_statusMessage == message)
    {
        return;
    }
    m_statusMessage = message;
    emit statusMessageChanged();
}

void VehicleController::setVehicles(const std::vector<Vehicle>& vehicles)
{
    m_vehicles.clear();
    m_vehicles.reserve(static_cast<int>(vehicles.size()));

    for(const Vehicle& vehicle : vehicles)
    {
        m_vehicles.append(QVariantMap{
            {QStringLiteral("id"), vehicle.id},
            {QStringLiteral("chassisCode"), QString::fromStdString(vehicle.chassisCode)},
            {QStringLiteral("driverId"), vehicle.driverId},
        });
    }
    emit vehiclesChanged();
}

void VehicleController::setSelectedSetup(const QVariantMap& setup)
{
    if(m_selectedSetup == setup)
    {
        return;
    }
    m_selectedSetup = setup;
    emit selectedSetupChanged();
}

void VehicleController::setInstalledParts(const std::vector<InstalledPart>& parts)
{
    m_installedParts.clear();
    m_installedParts.reserve(static_cast<int>(parts.size()));

    for(const InstalledPart& part : parts)
    {
        m_installedParts.append(QVariantMap{
            {QStringLiteral("partId"), part.partId},
            {QStringLiteral("name"), QString::fromStdString(part.name)},
            {QStringLiteral("category"), QString::fromStdString(part.category)},
            {QStringLiteral("installedAt"), QString::fromStdString(part.installedAt)},
            {QStringLiteral("wearPercent"), part.wearPercent},
        });
    }
    emit installedPartsChanged();
}
