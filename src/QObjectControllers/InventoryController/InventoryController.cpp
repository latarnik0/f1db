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

QString InventoryController::statusMessage() const
{
    return m_statusMessage;
}

void InventoryController::refresh()
{
    setParts(m_queryHandler.getAllParts());
    setStatusMessage(QStringLiteral("Loaded %1 parts").arg(m_parts.size()));
}

bool InventoryController::addPart(const QString& partName, const QString& category, double wearPercent)
{
    if(partName.trimmed().isEmpty() || category.trimmed().isEmpty())
    {
        setStatusMessage(QStringLiteral("Name and category required"));
        return false;
    }

    if(wearPercent < 0.0 || wearPercent > 100.0)
    {
        setStatusMessage(QStringLiteral("Wear must be 0..100"));
        return false;
    }

    const int newId = m_queryHandler.addPart(
        partName.trimmed().toStdString(),
        category.trimmed().toStdString(),
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
            {QStringLiteral("partName"), QString::fromStdString(part.partName)},
            {QStringLiteral("category"), QString::fromStdString(part.category)},
            {QStringLiteral("wearPercent"), part.wearPercent},
        });
    }
    emit partsChanged();
}
