#pragma once
#include <sqlite3.h>
#include <string>

class PersonnelDAO
{
private:
    sqlite3* db;

public:
    PersonnelDAO(sqlite3* db_connection);
    ~PersonnelDAO();

    void addDriver(const std::string& firstName, const std::string& lastName, const int& experienceLvl);
    void addEngineer(const std::string& firstName, const std::string& lastName, const std::string& division);
};