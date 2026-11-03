#pragma once

#include "QueryHandler/QueryHandler.hpp"
#include <QObject>
#include <QString>
#include <QVariantList>

class InventoryController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList parts READ parts NOTIFY partsChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit InventoryController(QueryHandler& queryHandler, QObject* parent = nullptr);

    QVariantList parts() const;
    QString statusMessage() const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool addPart(const QString& partName, const QString& category, double wearPercent);
    Q_INVOKABLE bool updatePartWear(int partId, double wearPercent);
    Q_INVOKABLE bool removePart(int partId);
    Q_INVOKABLE void filterByCategory(const QString& category);

signals:
    void partsChanged();
    void statusMessageChanged();

private:
    void setStatusMessage(const QString& message);
    void setParts(const std::vector<Part>& parts);

    QueryHandler& m_queryHandler;
    QVariantList m_parts;
    QString m_statusMessage;
};
