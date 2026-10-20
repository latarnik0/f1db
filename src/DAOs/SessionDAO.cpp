#include "DAOs/SessionDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"

SessionDAO::SessionDAO(sqlite3* db_connection) : db(db_connection)
{

}

SessionDAO::~SessionDAO()
{
    
}

void SessionDAO::addTrack(const std::string& name, const int& length)
{

}