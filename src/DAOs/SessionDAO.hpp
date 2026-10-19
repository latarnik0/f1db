#pragma once
#include "QueryHandler/QueryHandler.hpp"

class SessionDAO
{
private:
    sqlite3* db;

public:
    SessionDAO(sqlite3* db);
    ~SessionDAO();
};