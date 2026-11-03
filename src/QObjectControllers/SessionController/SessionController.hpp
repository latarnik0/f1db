#pragma once

#include "QueryHandler/QueryHandler.hpp"
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

class SessionController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList tracks READ tracks NOTIFY tracksChanged)
    Q_PROPERTY(QVariantList sessions READ sessions NOTIFY sessionsChanged)
    Q_PROPERTY(QVariantMap selectedSessionDetails READ selectedSessionDetails NOTIFY selectedSessionDetailsChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit SessionController(QueryHandler& queryHandler, QObject* parent = nullptr);

    QVariantList tracks() const;
    QVariantList sessions() const;
    QVariantMap selectedSessionDetails() const;
    QString statusMessage() const;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE bool addTrack(const QString& name, double lengthKm);
    Q_INVOKABLE bool removeTrack(int trackId);
    Q_INVOKABLE bool startSession(int trackId);
    Q_INVOKABLE void loadSessionDetails(int sessionId);

signals:
    void tracksChanged();
    void sessionsChanged();
    void selectedSessionDetailsChanged();
    void statusMessageChanged();

private:
    void setStatusMessage(const QString& message);
    void setTracks(const std::vector<Track>& tracks);
    void setSessions(const std::vector<RaceSession>& sessions);
    void setSelectedSessionDetails(const QVariantMap& details);

    QueryHandler& m_queryHandler;
    QVariantList m_tracks;
    QVariantList m_sessions;
    QVariantMap m_selectedSessionDetails;
    QString m_statusMessage;
};
