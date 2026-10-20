#include "InventoryDAO.hpp"
#include <string>

InventoryDAO::InventoryDAO(sqlite3* db_connection) : db(db_connection)
{

}

InventoryDAO::~InventoryDAO()
{
    
}