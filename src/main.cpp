#include "DatabaseManager/DatabaseManager.hpp"
#include "QueryHandler/QueryHandler.hpp"
#include "Mocks/MockReadings.hpp"

int main()
{
    DatabaseManager dbman("/home/latarnik3/studies/f1db/build/f1.db");
    sqlite3* dbcon = dbman.getConnection();
    QueryHandler qh(dbcon);
    //qh.addDriver("Max", "Verstappen", 5);
    //qh.addEngineer("Jan", "Kowalski", "aerodynmics");
    MockReadings mock;
    std::vector<std::vector<SensorReading>> sensorData = mock.prepareMockReadings();
    qh.insertMockReadings(sensorData);
    return 0;
}