#include "InventoryDAO.hpp"
#include "Logger/Logger.hpp"
#include <string>

InventoryDAO::InventoryDAO(sqlite3* db_connection) : db(db_connection) {}
InventoryDAO::~InventoryDAO() {}

int InventoryDAO::addPart(const std::string& partName, const std::string& category)
{
    const char* sql = "INSERT INTO parts_inventory (part_name, category) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, partName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_DONE) 
        {
            newId = sqlite3_last_insert_rowid(db);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return newId;
}

bool InventoryDAO::deletePart(const int& partId)
{
    const char* sql = "DELETE FROM parts_inventory WHERE part_id = ?;";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, partId);
        sqlite3_step(stmt);
        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            if(sqlite3_changes(db) > 0) 
            {
                result = true;
            }
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

Part InventoryDAO::getPartById(const int& partId)
{
    const char* sql = "SELECT FROM parts_inventory WHERE part_id = ?;";
    sqlite3_stmt* stmt;
    Part p = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, partId);
        sqlite3_step(stmt);
        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            Logger::getInstance().log(INFO, "Success");
        }
    }
    sqlite3_finalize(stmt);
    return p;
}

std::vector<Part> InventoryDAO::getPartByPartName(const std::string& partName)
{
    const char* sql = "SELECT FROM parts_inventory WHERE part_name = ?;";
    sqlite3_stmt* stmt;
    std::vector<Part> p = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, partName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            Logger::getInstance().log(INFO, "Success");
        }
    }
    sqlite3_finalize(stmt);
    return p;
}

std::vector<Part> InventoryDAO::getPartByCategory(const std::string& category)
{
    const char* sql = "SELECT FROM parts_inventory WHERE category = ?;";
    sqlite3_stmt* stmt;
    std::vector<Part> p = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            Logger::getInstance().log(INFO, "Success");
        }
    }
    sqlite3_finalize(stmt);
    return p;
}