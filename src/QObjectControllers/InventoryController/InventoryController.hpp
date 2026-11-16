#pragma once

#include "QueryHandler/QueryHandler.hpp"
#include <QObject>
#include <QString>
#include <QVariantList>

class InventoryController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList parts READ parts NOTIFY partsChanged)
    Q_PROPERTY(QVariantList criticalWearParts READ criticalWearParts NOTIFY criticalWearPartsChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit InventoryController(QueryHandler& queryHandler, QObject* parent = nullptr);

    QVariantList parts() const;
    QVariantList criticalWearParts() const;
    QString statusMessage() const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool addPart(const QString& name, const QString& category, int stockQuantity, double wearPercent);
    Q_INVOKABLE bool updatePartWear(int partId, double wearPercent);
    Q_INVOKABLE bool removePart(int partId);
    Q_INVOKABLE void filterByCategory(const QString& category);
    Q_INVOKABLE void queryInstalledPartsWear(double minWear, const QString& category);

signals:
    void partsChanged();
    void criticalWearPartsChanged();
    void statusMessageChanged();

private:
    void setStatusMessage(const QString& message);
    void setParts(const std::vector<Part>& parts);
    void setCriticalWearParts(const std::vector<CriticalWearPart>& parts);

    QueryHandler& m_queryHandler;
    QVariantList m_parts;
    QVariantList m_criticalWearParts;
    QString m_statusMessage;
};
