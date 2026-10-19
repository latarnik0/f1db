#include "DAOs/PersonnelDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"

PersonnelDAO::PersonnelDAO(sqlite3* db_connection) : db(db_connection)
{

}

PersonnelDAO::~PersonnelDAO()
{
    
}