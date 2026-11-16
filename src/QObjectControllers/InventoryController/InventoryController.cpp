#include "InventoryController.hpp"
#include <QVariantMap>

InventoryController::InventoryController(QueryHandler& queryHandler, QObject* parent)
    : QObject(parent)
    , m_queryHandler(queryHandler)
{
    refresh();
}

QVariantList InventoryController::parts() const
{
    return m_parts;
}

QVariantList InventoryController::criticalWearParts() const
{
    return m_criticalWearParts;
}

QString InventoryController::statusMessage() const
{
    return m_statusMessage;
}

void InventoryController::refresh()
{
    setParts(m_queryHandler.getAllParts());
    setStatusMessage(QStringLiteral("Loaded %1 parts").arg(m_parts.size()));
}

bool InventoryController::addPart(const QString& name, const QString& category, int stockQuantity, double wearPercent)
{
    if(name.trimmed().isEmpty() || category.trimmed().isEmpty())
    {
        setStatusMessage(QStringLiteral("Name and category required"));
        return false;
    }

    if(stockQuantity < 0)
    {
        setStatusMessage(QStringLiteral("Stock must be >= 0"));
        return false;
    }

    if(wearPercent < 0.0 || wearPercent > 100.0)
    {
        setStatusMessage(QStringLiteral("Wear must be 0..100"));
        return false;
    }

    const int newId = m_queryHandler.addPart(
        name.trimmed().toStdString(),
        category.trimmed().toStdString(),
        stockQuantity,
        wearPercent);

    if(newId < 0)
    {
        setStatusMessage(QStringLiteral("Failed to add part"));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Added part #%1").arg(newId));
    return true;
}

bool InventoryController::updatePartWear(int partId, double wearPercent)
{
    if(partId <= 0)
    {
        setStatusMessage(QStringLiteral("Part id must be > 0"));
        return false;
    }

    if(wearPercent < 0.0 || wearPercent > 100.0)
    {
        setStatusMessage(QStringLiteral("Wear must be 0..100"));
        return false;
    }

    if(!m_queryHandler.updatePartWear(partId, wearPercent))
    {
        setStatusMessage(QStringLiteral("Failed to update wear for part #%1").arg(partId));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Updated wear for part #%1").arg(partId));
    return true;
}

bool InventoryController::removePart(int partId)
{
    if(!m_queryHandler.deletePart(partId))
    {
        setStatusMessage(QStringLiteral("Failed to delete part #%1").arg(partId));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Deleted part #%1").arg(partId));
    return true;
}

void InventoryController::filterByCategory(const QString& category)
{
    if(category.trimmed().isEmpty())
    {
        refresh();
        return;
    }
    setParts(m_queryHandler.getPartByCategory(category.trimmed().toStdString()));
    setStatusMessage(QStringLiteral("Filter: %1 (%2)").arg(category).arg(m_parts.size()));
}

void InventoryController::queryInstalledPartsWear(double minWear, const QString& category)
{
    if(category.trimmed().isEmpty())
    {
        setCriticalWearParts({});
        setStatusMessage(QStringLiteral("Category required"));
        return;
    }

    const std::vector<CriticalWearPart> rows =
        m_queryHandler.queryInstalledPartsWear(minWear, category.trimmed().toStdString());
    setCriticalWearParts(rows);
    setStatusMessage(QStringLiteral("Critical wear: %1 part(s)").arg(m_criticalWearParts.size()));
}

void InventoryController::setStatusMessage(const QString& message)
{
    if(m_statusMessage == message)
    {
        return;
    }
    m_statusMessage = message;
    emit statusMessageChanged();
}

void InventoryController::setParts(const std::vector<Part>& parts)
{
    m_parts.clear();
    m_parts.reserve(static_cast<int>(parts.size()));

    for(const Part& part : parts)
    {
        m_parts.append(QVariantMap{
            {QStringLiteral("id"), part.id},
            {QStringLiteral("name"), QString::fromStdString(part.name)},
            {QStringLiteral("category"), QString::fromStdString(part.category)},
            {QStringLiteral("stockQuantity"), part.stockQuantity},
            {QStringLiteral("wearPercent"), part.wearPercent},
        });
    }
    emit partsChanged();
}

void InventoryController::setCriticalWearParts(const std::vector<CriticalWearPart>& parts)
{
    m_criticalWearParts.clear();
    m_criticalWearParts.reserve(static_cast<int>(parts.size()));

    for(const CriticalWearPart& part : parts)
    {
        m_criticalWearParts.append(QVariantMap{
            {QStringLiteral("name"), QString::fromStdString(part.name)},
            {QStringLiteral("wearPercentage"), part.wearPercentage},
            {QStringLiteral("chassisCode"), QString::fromStdString(part.chassisCode)},
        });
    }
    emit criticalWearPartsChanged();
}
