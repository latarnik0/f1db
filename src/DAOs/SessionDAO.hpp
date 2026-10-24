#pragma once
#include "Models/Models.hpp"
#include <sqlite3.h> 
#include <string>

class SessionDAO
{
private:
    sqlite3* db;

public:
    SessionDAO(sqlite3* db_connection);
    ~SessionDAO();

    int addTrack(const std::string& name, double lengthKm);    
    std::vector<Track> getAllTracks();
    bool deleteTrack(int trackId);
    int startSession(int trackId);
    std::vector<RaceSession> getAllSessions();
    SessionDetails getSessionDetails(int sessionId);
};