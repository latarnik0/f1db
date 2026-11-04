#include "PersonnelController.hpp"
#include <QVariantMap>

PersonnelController::PersonnelController(QueryHandler& queryHandler, QObject* parent)
    : QObject(parent)
    , m_queryHandler(queryHandler)
{
    refresh();
}

QVariantList PersonnelController::drivers() const
{
    return m_drivers;
}

QVariantList PersonnelController::engineers() const
{
    return m_engineers;
}

QString PersonnelController::statusMessage() const
{
    return m_statusMessage;
}

void PersonnelController::refresh()
{
    setDrivers(m_queryHandler.getAllDrivers());
    setEngineers(m_queryHandler.getAllEngineers());
    setStatusMessage(QStringLiteral("Loaded %1 drivers, %2 engineers")
                         .arg(m_drivers.size())
                         .arg(m_engineers.size()));
}

bool PersonnelController::addDriver(const QString& firstName, const QString& lastName, int experienceLvl)
{
    if(firstName.trimmed().isEmpty() || lastName.trimmed().isEmpty())
    {
        setStatusMessage(QStringLiteral("First and last name required"));
        return false;
    }

    if(experienceLvl < 0)
    {
        setStatusMessage(QStringLiteral("Experience level must be >= 0"));
        return false;
    }

    const int newId = m_queryHandler.addDriver(
        firstName.trimmed().toStdString(),
        lastName.trimmed().toStdString(),
        experienceLvl);

    if(newId < 0)
    {
        setStatusMessage(QStringLiteral("Failed to add driver"));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Added driver #%1").arg(newId));
    return true;
}

bool PersonnelController::removeDriver(int driverId)
{
    if(!m_queryHandler.deleteDriver(driverId))
    {
        setStatusMessage(QStringLiteral("Failed to delete driver #%1").arg(driverId));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Deleted driver #%1").arg(driverId));
    return true;
}

bool PersonnelController::updateDriverExperience(int driverId, int newLvl)
{
    if(newLvl < 0)
    {
        setStatusMessage(QStringLiteral("Experience level must be >= 0"));
        return false;
    }

    if(!m_queryHandler.updateDriverExperience(driverId, newLvl))
    {
        setStatusMessage(QStringLiteral("Failed to update driver #%1").arg(driverId));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Updated driver #%1 experience to %2").arg(driverId).arg(newLvl));
    return true;
}

void PersonnelController::filterDriversByLastName(const QString& lastName)
{
    if(lastName.trimmed().isEmpty())
    {
        setDrivers(m_queryHandler.getAllDrivers());
        setStatusMessage(QStringLiteral("Loaded %1 drivers").arg(m_drivers.size()));
        return;
    }
    setDrivers(m_queryHandler.getDriverByLastName(lastName.trimmed().toStdString()));
    setStatusMessage(QStringLiteral("Filter drivers: %1 (%2)").arg(lastName).arg(m_drivers.size()));
}

void PersonnelController::filterDriversByExperience(int experienceLvl)
{
    setDrivers(m_queryHandler.getDriverByExpLvl(experienceLvl));
    setStatusMessage(QStringLiteral("Filter experience: %1 (%2)").arg(experienceLvl).arg(m_drivers.size()));
}

bool PersonnelController::addEngineer(const QString& firstName, const QString& lastName, const QString& division)
{
    if(firstName.trimmed().isEmpty() || lastName.trimmed().isEmpty() || division.trimmed().isEmpty())
    {
        setStatusMessage(QStringLiteral("First name, last name and division required"));
        return false;
    }

    const int newId = m_queryHandler.addEngineer(
        firstName.trimmed().toStdString(),
        lastName.trimmed().toStdString(),
        division.trimmed().toStdString());

    if(newId < 0)
    {
        setStatusMessage(QStringLiteral("Failed to add engineer"));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Added engineer #%1").arg(newId));
    return true;
}

bool PersonnelController::removeEngineer(int engineerId)
{
    if(!m_queryHandler.deleteEngineer(engineerId))
    {
        setStatusMessage(QStringLiteral("Failed to delete engineer #%1").arg(engineerId));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Deleted engineer #%1").arg(engineerId));
    return true;
}

void PersonnelController::filterEngineersByLastName(const QString& lastName)
{
    if(lastName.trimmed().isEmpty())
    {
        setEngineers(m_queryHandler.getAllEngineers());
        setStatusMessage(QStringLiteral("Loaded %1 engineers").arg(m_engineers.size()));
        return;
    }
    setEngineers(m_queryHandler.getEngineerByLastName(lastName.trimmed().toStdString()));
    setStatusMessage(QStringLiteral("Filter engineers: %1 (%2)").arg(lastName).arg(m_engineers.size()));
}

void PersonnelController::filterEngineersByDivision(const QString& division)
{
    if(division.trimmed().isEmpty())
    {
        setEngineers(m_queryHandler.getAllEngineers());
        setStatusMessage(QStringLiteral("Loaded %1 engineers").arg(m_engineers.size()));
        return;
    }
    setEngineers(m_queryHandler.getEngineerByDivision(division.trimmed().toStdString()));
    setStatusMessage(QStringLiteral("Filter division: %1 (%2)").arg(division).arg(m_engineers.size()));
}

void PersonnelController::setStatusMessage(const QString& message)
{
    if(m_statusMessage == message)
    {
        return;
    }
    m_statusMessage = message;
    emit statusMessageChanged();
}

void PersonnelController::setDrivers(const std::vector<Driver>& drivers)
{
    m_drivers.clear();
    m_drivers.reserve(static_cast<int>(drivers.size()));

    for(const Driver& driver : drivers)
    {
        m_drivers.append(QVariantMap{
            {QStringLiteral("id"), driver.id},
            {QStringLiteral("firstName"), QString::fromStdString(driver.firstName)},
            {QStringLiteral("lastName"), QString::fromStdString(driver.lastName)},
            {QStringLiteral("experienceLvl"), driver.experienceLvl},
        });
    }
    emit driversChanged();
}

void PersonnelController::setEngineers(const std::vector<Engineer>& engineers)
{
    m_engineers.clear();
    m_engineers.reserve(static_cast<int>(engineers.size()));

    for(const Engineer& engineer : engineers)
    {
        m_engineers.append(QVariantMap{
            {QStringLiteral("id"), engineer.id},
            {QStringLiteral("firstName"), QString::fromStdString(engineer.firstName)},
            {QStringLiteral("lastName"), QString::fromStdString(engineer.lastName)},
            {QStringLiteral("division"), QString::fromStdString(engineer.division)},
        });
    }
    emit engineersChanged();
}
