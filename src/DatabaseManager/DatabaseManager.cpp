#include "DatabaseManager/DatabaseManager.hpp"
#include <iostream>
#include <sqlite3.h>

DatabaseManager::DatabaseManager(const char* db_path)
{
    if(sqlite3_open(db_path, &db) != SQLITE_OK)
    {
        Logger::getInstance().log(CRITICAL, "Cannot open/create database!\n");
        sqlite3_close(db); 
    } 
    else 
    {
        Logger::getInstance().log(INFO, "Connected with database.\n");
    }
}
    
DatabaseManager::~DatabaseManager() 
{
    if(db) 
    {
        sqlite3_close(db);
        Logger::getInstance().log(INFO, "Closed connection with database.\n");
    }
}

sqlite3* DatabaseManager::getConnection()
{
    return db;
}

