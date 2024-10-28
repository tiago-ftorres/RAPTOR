#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "DataStructures.h"

std::unordered_map<int, Agency> readAgencies(const std::string& filename);
std::unordered_map<int, Calendar> readCalendars(const std::string& filename);
std::unordered_map<int, Route> readRoutes(const std::string& filename);
std::unordered_map<int, Stop> readStops(const std::string& filename);
std::unordered_map<int, Trip> readTrips(const std::string& filename);
std::vector<StopTime> readStopTimes(const std::string& filename);
void cleanLine(std::string& line);

#endif //PARSE_H
