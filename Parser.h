#ifndef PARSE_H
#define PARSE_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>

#include "Utils.h"

#include "NetworkObjects/DataStructures.h"
#include "NetworkObjects/GTFSObjects/Agency.h"
#include "NetworkObjects/GTFSObjects/Calendar.h"
#include "NetworkObjects/GTFSObjects/Route.h"
#include "NetworkObjects/GTFSObjects/Stop.h"
#include "NetworkObjects/GTFSObjects/Trip.h"
#include "NetworkObjects/GTFSObjects/StopTime.h"

class Parser {
  private:
    std::string inputDirectory;

    std::unordered_map<std::string, Agency> agencies_;
    std::unordered_map<std::string, Calendar> calendars_;
    std::unordered_map<std::string, Stop> stops_;
    std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> routes_; // Key is (route_id, direction_id)
    std::unordered_map<std::string, Trip> trips_;
    std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> stop_times_; // key is (trip_id, stop_id)

    void parseAgencies();
    void parseCalendars();
    void parseRoutes();
    void parseStops();
    void parseTrips();
    void parseStopTimes();

    void associateData();

public:
    explicit Parser(std::string  directory);

    [[nodiscard]] std::unordered_map<std::string, Agency> getAgencies();
    [[nodiscard]] std::unordered_map<std::string, Calendar> getCalendars();
    [[nodiscard]] std::unordered_map<std::string, Stop> getStops();
    [[nodiscard]] std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> getRoutes(); // Key is (route_id, direction_id)
    [[nodiscard]] std::unordered_map<std::string, Trip> getTrips();
    [[nodiscard]] std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> getStopTimes(); // key is (trip_id, stop_id)

  static void cleanLine(std::string &line) ;
};

#endif //PARSE_H
