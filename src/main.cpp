#include "QueryHandler.hpp"
#include "Logger/Logger.hpp"
#include "DatabaseManager/DatabaseManager.hpp"
#include "InventoryController/InventoryController.hpp"
#include <sqlite3.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[]) 
{
    QGuiApplication f1db_app(argc, argv);

    DatabaseManager dbman("f1.db");
    QueryHandler qh(dbman.getConnection());
    InventoryController inventoryController(qh);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("inventoryController", &inventoryController);
    engine.loadFromModule("F1DB", "Main");

    return f1db_app.exec();
}
