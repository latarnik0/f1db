#pragma once
#include "QueryHandler/QueryHandler.hpp"

class PersonnelDAO
{
private:
    sqlite3* db;

public:
    PersonnelDAO(sqlite3* db);
    ~PersonnelDAO();
};