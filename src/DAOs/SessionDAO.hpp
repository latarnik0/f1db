#pragma once
#include <sqlite3.h> 

class SessionDAO
{
private:
    sqlite3* db;

public:
    SessionDAO(sqlite3* db_connection);
    ~SessionDAO();
};