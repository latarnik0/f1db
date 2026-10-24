#pragma once
#include "Models/Models.hpp"
#include <sqlite3.h>
#include <string>
#include <vector>

class PersonnelDAO
{
private:
    sqlite3* db;

public:
    PersonnelDAO(sqlite3* db_connection);
    ~PersonnelDAO();

    // Drivers
    int addDriver(const std::string& firstName, const std::string& lastName, const int& experienceLvl);
    bool deleteDriver(const int& driverId);
    Driver getDriverById(const int& driverId);
    std::vector<Driver> getAllDrivers();
    std::vector<Driver> getDriverByLastName(const std::string& lastName);
    std::vector<Driver> getDriverByExpLvl(const int& experienceLvl);
    bool updateDriverExperience(const int& driverId, const int& newLvl);

    // Engineers
    int addEngineer(const std::string& firstName, const std::string& lastName, const std::string& division);
    bool deleteEngineer(const int& engineerId);
    Engineer getEngineerById(const int& engineerId);
    std::vector<Engineer> getAllEngineers();
    std::vector<Engineer> getEngineerByLastName(const std::string& lastName);
    std::vector<Engineer> getEngineerByDivision(const std::string& division);
};