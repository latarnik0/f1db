#include "mocks/sensory_readings.hpp"
#include <iostream>
#include <sqlite3.h>

class DatabaseManager
{
private:
    sqlite3* db;

public:
    DatabaseManager(const char* db_path)
    {
        if(sqlite3_open(db_path, &db) != SQLITE_OK)
        {
            std::cerr << "[ERROR] Cannot open database: " << sqlite3_errmsg(db) << '\n';
            sqlite3_close(db); 
        } 
        else 
        {
            std::cout << "[OK] Connected with database.\n";
        }
    }
    ~DatabaseManager() 
    {
        if(db) 
        {
            sqlite3_close(db);
            std::cout << "[OK] Closed connection with database.\n";
        }
    }

    void executeQuery(const char* sql)
    {
    
    }
};

int main()
{
    DatabaseManager dbman("f1.db");
    return 0;
}