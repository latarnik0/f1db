#include "InventoryDAO.hpp"
#include <string>

InventoryDAO::InventoryDAO(sqlite3* db_connection) : db(db_connection)
{

}

InventoryDAO::~InventoryDAO()
{
    
}

int InventoryDAO::addPart(const std::string& partName, const std::string& partType)
{
    const char* sql = "INSERT INTO engineers (part_name, part_type) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, partName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, partType.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        if (sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
        }
    }
    sqlite3_finalize(stmt);
    return newId;
}