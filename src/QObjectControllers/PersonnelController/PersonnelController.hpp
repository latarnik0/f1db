#pragma once

#include "QueryHandler/QueryHandler.hpp"
#include <QObject>
#include <QString>
#include <QVariantList>

class PersonnelController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList drivers READ drivers NOTIFY driversChanged)
    Q_PROPERTY(QVariantList engineers READ engineers NOTIFY engineersChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit PersonnelController(QueryHandler& queryHandler, QObject* parent = nullptr);

    QVariantList drivers() const;
    QVariantList engineers() const;
    QString statusMessage() const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool addDriver(const QString& firstName, const QString& lastName, int experienceLvl);
    Q_INVOKABLE bool removeDriver(int driverId);
    Q_INVOKABLE bool updateDriverExperience(int driverId, int newLvl);
    Q_INVOKABLE void filterDriversByLastName(const QString& lastName);
    Q_INVOKABLE void filterDriversByExperience(int experienceLvl);
    Q_INVOKABLE bool addEngineer(const QString& firstName, const QString& lastName, const QString& division);
    Q_INVOKABLE bool removeEngineer(int engineerId);
    Q_INVOKABLE void filterEngineersByLastName(const QString& lastName);
    Q_INVOKABLE void filterEngineersByDivision(const QString& division);

signals:
    void driversChanged();
    void engineersChanged();
    void statusMessageChanged();

private:
    void setStatusMessage(const QString& message);
    void setDrivers(const std::vector<Driver>& drivers);
    void setEngineers(const std::vector<Engineer>& engineers);

    QueryHandler& m_queryHandler;
    QVariantList m_drivers;
    QVariantList m_engineers;
    QString m_statusMessage;
};
