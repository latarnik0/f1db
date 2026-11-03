#include "SessionController.hpp"

SessionController::SessionController(QueryHandler& queryHandler, QObject* parent)
    : QObject(parent)
    , m_queryHandler(queryHandler)
{
    refresh();
}

QVariantList SessionController::tracks() const
{
    return m_tracks;
}

QVariantList SessionController::sessions() const
{
    return m_sessions;
}

QVariantMap SessionController::selectedSessionDetails() const
{
    return m_selectedSessionDetails;
}

QString SessionController::statusMessage() const
{
    return m_statusMessage;
}

void SessionController::refresh()
{
    setTracks(m_queryHandler.getAllTracks());
    setSessions(m_queryHandler.getAllSessions());
    setSelectedSessionDetails({});
    setStatusMessage(QStringLiteral("Loaded %1 tracks, %2 sessions")
                         .arg(m_tracks.size())
                         .arg(m_sessions.size()));
}

bool SessionController::addTrack(const QString& name, double lengthKm)
{
    if(name.trimmed().isEmpty())
    {
        setStatusMessage(QStringLiteral("Track name required"));
        return false;
    }

    if(lengthKm <= 0)
    {
        setStatusMessage(QStringLiteral("Length must be > 0"));
        return false;
    }

    const int newId = m_queryHandler.addTrack(
        name.trimmed().toStdString(),
        lengthKm);

    if(newId < 0)
    {
        setStatusMessage(QStringLiteral("Failed to add track"));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Added track #%1").arg(newId));
    return true;
}

bool SessionController::removeTrack(int trackId)
{
    if(!m_queryHandler.deleteTrack(trackId))
    {
        setStatusMessage(QStringLiteral("Failed to delete track #%1").arg(trackId));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Deleted track #%1").arg(trackId));
    return true;
}

bool SessionController::startSession(int trackId)
{
    const int newId = m_queryHandler.startSession(trackId);

    if(newId < 0)
    {
        setStatusMessage(QStringLiteral("Failed to start session"));
        return false;
    }
    refresh();
    setStatusMessage(QStringLiteral("Started session #%1").arg(newId));
    return true;
}

void SessionController::loadSessionDetails(int sessionId)
{
    const SessionDetails details = m_queryHandler.getSessionDetails(sessionId);

    if(details.sessionId <= 0)
    {
        setSelectedSessionDetails({});
        setStatusMessage(QStringLiteral("Failed to load session #%1").arg(sessionId));
        return;
    }

    setSelectedSessionDetails(QVariantMap{
        {QStringLiteral("sessionId"), details.sessionId},
        {QStringLiteral("sessionDate"), QString::fromStdString(details.sessionDate)},
        {QStringLiteral("trackId"), details.trackId},
        {QStringLiteral("trackName"), QString::fromStdString(details.trackName)},
        {QStringLiteral("trackLengthKm"), details.trackLengthKm},
    });
    setStatusMessage(QStringLiteral("Loaded session #%1 details").arg(details.sessionId));
}

void SessionController::setStatusMessage(const QString& message)
{
    if(m_statusMessage == message)
    {
        return;
    }
    m_statusMessage = message;
    emit statusMessageChanged();
}

void SessionController::setTracks(const std::vector<Track>& tracks)
{
    m_tracks.clear();
    m_tracks.reserve(static_cast<int>(tracks.size()));

    for(const Track& track : tracks)
    {
        m_tracks.append(QVariantMap{
            {QStringLiteral("id"), track.id},
            {QStringLiteral("name"), QString::fromStdString(track.name)},
            {QStringLiteral("lengthKm"), track.lengthKm},
        });
    }
    emit tracksChanged();
}

void SessionController::setSessions(const std::vector<RaceSession>& sessions)
{
    m_sessions.clear();
    m_sessions.reserve(static_cast<int>(sessions.size()));

    for(const RaceSession& session : sessions)
    {
        m_sessions.append(QVariantMap{
            {QStringLiteral("id"), session.id},
            {QStringLiteral("trackId"), session.trackId},
            {QStringLiteral("sessionDate"), QString::fromStdString(session.sessionDate)},
        });
    }
    emit sessionsChanged();
}

void SessionController::setSelectedSessionDetails(const QVariantMap& details)
{
    if(m_selectedSessionDetails == details)
    {
        return;
    }
    m_selectedSessionDetails = details;
    emit selectedSessionDetailsChanged();
}
