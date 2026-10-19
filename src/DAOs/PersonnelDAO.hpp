#pragma once
#include <sqlite3.h> 

class PersonnelDAO
{
private:
    sqlite3* db;

public:
    PersonnelDAO(sqlite3* db_connection);
    ~PersonnelDAO();
};