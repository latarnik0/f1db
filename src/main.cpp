#include "QueryHandler.hpp"
#include "Logger/Logger.hpp"
#include "DatabaseManager/DatabaseManager.hpp"
#include <sqlite3.h>

int main() 
{
    DatabaseManager dbman("f1.db");
    sqlite3* db;
    QueryHandler qh(db);
    return 0;
}