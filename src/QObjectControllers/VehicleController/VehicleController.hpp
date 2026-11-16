#pragma once

#include "QueryHandler/QueryHandler.hpp"
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

class VehicleController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList vehicles READ vehicles NOTIFY vehiclesChanged)
    Q_PROPERTY(QVariantMap selectedSetup READ selectedSetup NOTIFY selectedSetupChanged)
    Q_PROPERTY(QVariantList installedParts READ installedParts NOTIFY installedPartsChanged)
    Q_PROPERTY(QVariantList setupComparison READ setupComparison NOTIFY setupComparisonChanged)
    Q_PROPERTY(QVariantMap comparisonMeta READ comparisonMeta NOTIFY comparisonMetaChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit VehicleController(QueryHandler& queryHandler, QObject* parent = nullptr);

    QVariantList vehicles() const;
    QVariantMap selectedSetup() const;
    QVariantList installedParts() const;
    QVariantList setupComparison() const;
    QVariantMap comparisonMeta() const;
    QString statusMessage() const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool addVehicle(const QString& chassisCode, int driverId);
    Q_INVOKABLE bool removeVehicle(int vehicleId);
    Q_INVOKABLE bool setRunSetup(int runId, double frontWingAngle, double rearWingAngle, double brakeBias, double suspensionStiffness);
    Q_INVOKABLE void loadSetup(int runId);
    Q_INVOKABLE void compareSetups(int runIdA, int runIdB);
    Q_INVOKABLE bool installPart(int vehicleId, int partId);
    Q_INVOKABLE bool removePart(int vehicleId, int partId);
    Q_INVOKABLE void loadInstalledParts(int vehicleId);

signals:
    void vehiclesChanged();
    void selectedSetupChanged();
    void installedPartsChanged();
    void setupComparisonChanged();
    void comparisonMetaChanged();
    void statusMessageChanged();

private:
    void setStatusMessage(const QString& message);
    void setVehicles(const std::vector<Vehicle>& vehicles);
    void setSelectedSetup(const QVariantMap& setup);
    void setInstalledParts(const std::vector<InstalledPart>& parts);
    void setSetupComparison(const QVariantList& rows);
    void setComparisonMeta(const QVariantMap& meta);

    QueryHandler& m_queryHandler;
    QVariantList m_vehicles;
    QVariantList m_installedParts;
    QVariantList m_setupComparison;
    QVariantMap m_selectedSetup;
    QVariantMap m_comparisonMeta;
    QString m_statusMessage;
};
