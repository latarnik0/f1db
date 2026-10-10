#include "DatabaseManager/DatabaseManager.hpp"

int main()
{
    DatabaseManager dbman("/home/latarnik3/f1db/build/f1.db");
    dbman.insertMockReadings();
    return 0;
}