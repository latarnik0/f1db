#pragma once
#include "Models/Models.hpp"
#include <sqlite3.h> 
#include <vector>
#include <string>

class SessionDAO
{
private:
    sqlite3* db;

public:
    SessionDAO(sqlite3* db_connection);
    ~SessionDAO();

    int addTrack(const std::string& name, const std::string& location);    
    std::vector<Track> getAllTracks();
    bool deleteTrack(int trackId);
    int startSession(int trackId, const std::string& sessionType, const std::string& weather);
    std::vector<RaceSession> getAllSessions();
    SessionDetails getSessionDetails(int sessionId);
};
