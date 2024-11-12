#ifndef PARSE_H
#define PARSE_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include "DataStructures.h"

class Parser {
  private:
    std::string inputDirectory;

  public:
    explicit Parser(std::string  directory) : inputDirectory(std::move(directory)) {}

    [[nodiscard]] std::unordered_map<std::string, Agency> readAgencies() const;
    [[nodiscard]] std::unordered_map<std::string, Calendar> readCalendars() const;
    [[nodiscard]] std::unordered_map<std::string, Route> readRoutes() const;
    [[nodiscard]] std::unordered_map<std::string, Stop> readStops() const;
    [[nodiscard]] std::unordered_map<std::string, Trip> readTrips() const;
    [[nodiscard]] std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> readStopTimes() const;
    static void cleanLine(std::string &line) ;
};

#endif //PARSE_H
