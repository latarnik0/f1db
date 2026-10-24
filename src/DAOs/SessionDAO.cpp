#include "Logger/Logger.hpp"
#include "DAOs/SessionDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"

SessionDAO::SessionDAO(sqlite3* db_connection) : db(db_connection) {}
SessionDAO::~SessionDAO() {}

int SessionDAO::addTrack(const std::string& name, double lengthKm)
{
    const char* sql = "INSERT INTO tracks (name, length_km) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 2, lengthKm);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
        }
    }
    else 
    { 
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db)); 
    }
    sqlite3_finalize(stmt);
    return newId;
}

std::vector<Track> SessionDAO::getAllTracks()
{
    const char* sql = "SELECT track_id, name, length_km FROM tracks;";
    sqlite3_stmt* stmt;
    std::vector<Track> tracksList;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Track t;
            t.id = sqlite3_column_int(stmt, 0);
            t.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            t.lengthKm = sqlite3_column_double(stmt, 2);
            tracksList.push_back(t);
        }
    }
    else
    { 
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db)); 
    }
    sqlite3_finalize(stmt);
    return tracksList;
}

bool SessionDAO::deleteTrack(int trackId)
{
    const char* sql = "DELETE FROM tracks WHERE track_id = ?;";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, trackId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            if(sqlite3_changes(db) > 0)
            {
                result = true;
            }
        }
    }
    else
    { 
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db)); 
    }
    sqlite3_finalize(stmt);
    return result;
}

int SessionDAO::startSession(int trackId)
{
    const char* sql = "INSERT INTO race_sessions (track_id) VALUES (?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, trackId);

        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
        }
    }
    else
    { 
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db)); 
    }
    sqlite3_finalize(stmt);
    return newId;
}

std::vector<RaceSession> SessionDAO::getAllSessions()
{
    const char* sql = "SELECT session_id, track_id, session_date FROM race_sessions;";
    sqlite3_stmt* stmt;
    std::vector<RaceSession> sessionsList;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            RaceSession rs;
            rs.id = sqlite3_column_int(stmt, 0);
            rs.trackId = sqlite3_column_int(stmt, 1);
            rs.sessionDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            sessionsList.push_back(rs);
        }
    }
    else 
    { 
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return sessionsList;
}

SessionDetails SessionDAO::getSessionDetails(int sessionId)
{
    const char* sql = "SELECT rs.session_id, rs.session_date, t.track_id, t.name, t.length_km "
                      "FROM race_sessions rs "
                      "JOIN tracks t ON rs.track_id = t.track_id "
                      "WHERE rs.session_id = ?;";
    sqlite3_stmt* stmt;
    SessionDetails sd;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_int(stmt, 1, sessionId);

        if(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            sd.sessionId = sqlite3_column_int(stmt, 0);
            sd.sessionDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            sd.trackId = sqlite3_column_int(stmt, 2);
            sd.trackName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            sd.trackLengthKm = sqlite3_column_double(stmt, 4);  
            sqlite3_finalize(stmt);
            return sd;
        }
    }
    else
    { 
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return sd;
}