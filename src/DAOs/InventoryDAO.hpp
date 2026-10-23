#pragma once
#include "Models/Models.hpp"
#include <sqlite3.h>
#include <string>
#include <vector>

class InventoryDAO
{
private:
    sqlite3* db;

public:
    InventoryDAO(sqlite3* db_connection);
    ~InventoryDAO();

    int addPart(const std::string& partName, const std::string& category);
    bool deletePart(const int& partId);
    Part getPartById(const int& partId);
    std::vector<Part> getPartByPartName(const std::string& partName);
    std::vector<Part> getPartByCategory(const std::string& category);
    std::vector<Part> getAllParts();
};