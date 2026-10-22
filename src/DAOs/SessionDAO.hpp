#pragma once
#include <sqlite3.h> 
#include <string>

class SessionDAO
{
private:
    sqlite3* db;

public:
    SessionDAO(sqlite3* db_connection);
    ~SessionDAO();

    void addTrack(const std::string& name, const int& length);
};