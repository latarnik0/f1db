#include "QueryHandler.hpp"
#include "Logger/Logger.hpp"
#include "DatabaseManager/DatabaseManager.hpp"
#include <sqlite3.h>
#include <QGuiApplication>

int main() 
{
    DatabaseManager dbman("f1.db");
    QueryHandler qh(dbman.getConnection());
    return 0;
}