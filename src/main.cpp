#include "DatabaseManager/DatabaseManager.hpp"
#include "QueryHandler/QueryHandler.hpp"

int main()
{
    DatabaseManager dbman("/home/latarnik3/studies/f1db/build/f1.db");
    sqlite3* dbcon = dbman.getConnection();
    QueryHandler qh(dbcon);
    qh.addDriver("Max", "Verstappen", 5);
    qh.addEngineer("Jan", "Kowalski", "aerodynmics");
    return 0;
}