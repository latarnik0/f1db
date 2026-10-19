#include "InventoryDAO.hpp"

InventoryDAO::InventoryDAO(sqlite3* db_connection) : db(db_connection)
{

}

InventoryDAO::~InventoryDAO()
{
    
}