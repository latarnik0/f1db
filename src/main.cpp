#include "QueryHandler.hpp"
#include "Logger/Logger.hpp"
#include "DatabaseManager/DatabaseManager.hpp"
#include "InventoryController/InventoryController.hpp"
#include "PersonnelController/PersonnelController.hpp"
#include "SessionController/SessionController.hpp"
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
    PersonnelController personnelController(qh);
    SessionController sessionController(qh);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("inventoryController", &inventoryController);
    engine.rootContext()->setContextProperty("personnelController", &personnelController);
    engine.rootContext()->setContextProperty("sessionController", &sessionController);
    engine.loadFromModule("F1DB", "Main");

    return f1db_app.exec();
}
