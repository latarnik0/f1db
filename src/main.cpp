#include "QueryHandler.hpp"
#include "Logger/Logger.hpp"
#include "DatabaseManager/DatabaseManager.hpp"
#include <sqlite3.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) 
{
    QGuiApplication f1db_app(argc, argv);
    
    DatabaseManager dbman("f1.db");
    QueryHandler qh(dbman.getConnection());
    
    return f1db_app.exec();
}