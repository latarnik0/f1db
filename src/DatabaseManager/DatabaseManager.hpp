#include "mocks/sensory_readings.hpp"
#include <iostream>
#include <sqlite3.h>

class DatabaseManager
{
private:
    sqlite3* db;

public:
    DatabaseManager(const char* db_path) {};
    ~DatabaseManager() {} ;

    void insertMockReadings() {};
};