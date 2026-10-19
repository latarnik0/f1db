#include "DatabaseManager/DatabaseManager.hpp"
#include "QueryHandler/QueryHandler.hpp"
#include <iostream>
#include <sqlite3.h>

QueryHandler::QueryHandler(sqlite3* db_connection) : 
    inventory(db_connection),
    personnel(db_connection),
    session(db_connection),
    telemetry(db_connection),
    vehicle(db_connection)
{

}

QueryHandler::~QueryHandler()
{
    
}