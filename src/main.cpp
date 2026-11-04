#include "QueryHandler.hpp"
#include "Logger/Logger.hpp"
#include "DatabaseManager/DatabaseManager.hpp"
#include "InventoryController/InventoryController.hpp"
#include "PersonnelController/PersonnelController.hpp"
#include "SessionController/SessionController.hpp"
#include "TelemetryController/TelemetryController.hpp"
#include "VehicleController/VehicleController.hpp"
#include "AppState/AppState.hpp"
#include <sqlite3.h>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication f1db_app(argc, argv);
    QDir::setCurrent(f1db_app.applicationDirPath());

    const QString dbFile = QStringLiteral("f1.db");
    DatabaseManager dbman(dbFile.toUtf8().constData());
    QueryHandler qh(dbman.getConnection());
    InventoryController inventoryController(qh);
    PersonnelController personnelController(qh);
    SessionController sessionController(qh);
    TelemetryController telemetryController(qh);
    VehicleController vehicleController(qh);

    AppState appState;
    appState.setDbConnected(dbman.isConnected());
    appState.setDbPath(QDir::current().absoluteFilePath(dbFile));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appState", &appState);
    engine.rootContext()->setContextProperty("inventoryController", &inventoryController);
    engine.rootContext()->setContextProperty("personnelController", &personnelController);
    engine.rootContext()->setContextProperty("sessionController", &sessionController);
    engine.rootContext()->setContextProperty("telemetryController", &telemetryController);
    engine.rootContext()->setContextProperty("vehicleController", &vehicleController);
    engine.loadFromModule("F1DB", "Main");

    if(engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return f1db_app.exec();
}
