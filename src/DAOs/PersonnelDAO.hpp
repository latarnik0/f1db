#pragma once
#include <sqlite3.h>
#include <string>

class PersonnelDAO
{
private:
    sqlite3* db;

public:
    PersonnelDAO(sqlite3* db_connection);
    ~PersonnelDAO();

    // Drivers
    int addDriver(const std::string& firstName, const std::string& lastName, const int& experienceLvl);
    Driver getDriverById(int driverId);
    std::vector<Driver> getAllDrivers();
    std::vector<Driver> getDriverByLastName(const std::string& lastName);
    std::vector<Driver> getDriverByExpLvl(int experienceLvl);

    // Engineers
    int addEngineer(const std::string& firstName, const std::string& lastName, const std::string& division);
    bool deleteEngineer(int engineerId);
    Engineer getEngineerById(int engineerId);
    std::vector<Engineer> getAllEngineers();
    std::vector<Engineer> getEngineerByLastName(const std::string& lastName);
    std::vector<Engineer> getEngineerByDivision(const std::string& division);
};