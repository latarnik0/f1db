#include "DAOs/PersonnelDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"
#include <string>

PersonnelDAO::PersonnelDAO(sqlite3* db_connection) : db(db_connection)
{

}

PersonnelDAO::~PersonnelDAO()
{
    
}

int PersonnelDAO::addDriver(const std::string& firstName, const std::string& lastName, const int& experienceLvl)
{
    const char* sql = "INSERT INTO drivers (first_name, last_name, experience_lvl) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, experienceLvl);
        sqlite3_step(stmt);
        if (sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
        }
    }
    sqlite3_finalize(stmt);
    return newId;
}

int PersonnelDAO::addEngineer(const std::string& firstName, const std::string& lastName, const std::string& division)
{
    const char* sql = "INSERT INTO engineers (first_name, last_name, division) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) 
    {
        sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, division.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        if (sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
        }
    }
    sqlite3_finalize(stmt);
    return newId;
}

