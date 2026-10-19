#include "DAOs/VehicleDAO.hpp"
#include "QueryHandler/QueryHandler.hpp"

VehicleDAO::VehicleDAO(sqlite3* db_connection) : db(db_connection)
{

}

VehicleDAO::~VehicleDAO()
{
    
}