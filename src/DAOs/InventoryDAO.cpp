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
            Logger::getInstance().log(INFO, "Entry added");
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
        if(sqlite3_step(stmt) == SQLITE_DONE) 
        {
            if(sqlite3_changes(db) > 0) 
            {
                result = true;
                Logger::getInstance().log(INFO, "Entry deleted");
            }
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return result;
}

Part InventoryDAO::getPartById(const int& partId)
{
    const char* sql = "SELECT * FROM parts_inventory WHERE part_id = ?;";
    sqlite3_stmt* stmt;
    Part p = {};
    
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, partId);
        if(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            p.id = sqlite3_column_int(stmt, 0);
            p.partName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            p.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            sqlite3_finalize(stmt);
            return p;
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return p;
}

std::vector<Part> InventoryDAO::getPartByPartName(const std::string& partName)
{
    const char* sql = "SELECT * FROM parts_inventory WHERE part_name = ?;";
    sqlite3_stmt* stmt;
    std::vector<Part> partsList = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, partName.c_str(), -1, SQLITE_TRANSIENT);
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Part p;
            p.id = sqlite3_column_int(stmt, 0);
            p.partName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            p.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            partsList.push_back(p);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return partsList;
}

std::vector<Part> InventoryDAO::getPartByCategory(const std::string& category)
{
    const char* sql = "SELECT * FROM parts_inventory WHERE category = ?;";
    sqlite3_stmt* stmt;
    std::vector<Part> partsList = {};

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_TRANSIENT);
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Part p;
            p.id = sqlite3_column_int(stmt, 0);
            p.partName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            p.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            partsList.push_back(p);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return partsList;
}

std::vector<Part> InventoryDAO::getAllParts()
{
    const char* sql = "SELECT part_id, part_name, category FROM parts_inventory;";
    sqlite3_stmt* stmt;
    std::vector<Part> partsList;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while(sqlite3_step(stmt) == SQLITE_ROW) 
        {
            Part p;
            p.id = sqlite3_column_int(stmt, 0);
            p.partName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            p.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            partsList.push_back(p);
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return partsList;
}