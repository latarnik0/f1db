#include "DAOs/PersonnelDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"
#include <string>

PersonnelDAO::PersonnelDAO(sqlite3* db_connection) : db(db_connection)
{

}

PersonnelDAO::~PersonnelDAO()
{
    
}

void PersonnelDAO::addDriver(const std::string& firstName, const std::string& lastName, const int& experienceLvl)
{

}

void PersonnelDAO::addEngineer(const std::string& firstName, const std::string& lastName, const std::string& division)
{

}

