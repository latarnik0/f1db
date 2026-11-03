#include "InventoryDAO.hpp"
#include "Logger/Logger.hpp"
#include <string>

InventoryDAO::InventoryDAO(sqlite3* db_connection) : db(db_connection) {}
InventoryDAO::~InventoryDAO() {}

int InventoryDAO::addPart(const std::string& partName, const std::string& category, double wearPercent)
{
    if(wearPercent < 0.0 || wearPercent > 100.0)
    {
        Logger::getInstance().log(ERROR, "INVENTORY: wear_percent must be 0..100");
        return -1;
    }

    const char* sql = "INSERT INTO parts_inventory (part_name, category, wear_percent) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int newId = -1;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, partName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 3, wearPercent);
        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            newId = sqlite3_last_insert_rowid(db);
            Logger::getInstance().log(INFO, "INVENTORY: Entry added");
        }
    }
    else
    {
        Logger::getInstance().log(ERROR, sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return newId;
}

bool InventoryDAO::updatePartWear(int partId, double wearPercent)
{
    if(wearPercent < 0.0 || wearPercent > 100.0)
    {
        Logger::getInstance().log(ERROR, "INVENTORY: wear_percent must be 0..100");
        return false;
    }

    const char* sql = "UPDATE parts_inventory SET wear_percent = ? WHERE part_id = ?;";
    sqlite3_stmt* stmt;
    bool result = false;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_double(stmt, 1, wearPercent);
        sqlite3_bind_int(stmt, 2, partId);
        if(sqlite3_step(stmt) == SQLITE_DONE)
        {
            if(sqlite3_changes(db) > 0)
            {
                result = true;
                Logger::getInstance().log(INFO, "INVENTORY: Wear updated");
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
                Logger::getInstance().log(INFO, "INVENTORY: Entry deleted");
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
    const char* sql = "SELECT part_id, part_name, category, wear_percent FROM parts_inventory WHERE part_id = ?;";
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
            p.wearPercent = sqlite3_column_double(stmt, 3);
            sqlite3_finalize(stmt);
            Logger::getInstance().log(INFO, "INVENTORY: Entry retrieved");
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
    const char* sql = "SELECT part_id, part_name, category, wear_percent FROM parts_inventory WHERE part_name = ?;";
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
            p.wearPercent = sqlite3_column_double(stmt, 3);
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
    const char* sql = "SELECT part_id, part_name, category, wear_percent FROM parts_inventory WHERE category = ?;";
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
            p.wearPercent = sqlite3_column_double(stmt, 3);
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
    const char* sql = "SELECT part_id, part_name, category, wear_percent FROM parts_inventory;";
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
            p.wearPercent = sqlite3_column_double(stmt, 3);
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
