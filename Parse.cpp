#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "Parse.h"

std::unordered_map<int, Agency> readAgencies(const std::string &filePath) {
  std::unordered_map<int, Agency> agencies;
  std::ifstream file(filePath);
  std::string line;
  std::getline(file, line);  // Skip header
  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Agency agency;

    std::string agency_id_str;
    std::getline(ss, agency_id_str, ',');
    agency.agency_id = std::stoi(agency_id_str);

    std::getline(ss, agency.agency_name, ',');

    agencies[agency.agency_id] = agency;
  }
  return agencies;
}

std::unordered_map<int, Calendar> readCalendars(const std::string &filePath) {
  std::unordered_map<int, Calendar> calendars;
  std::ifstream file(filePath);
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Calendar calendar;

    std::string service_id_str;
    std::getline(ss, service_id_str, ',');
    calendar.service_id = std::stoi(service_id_str);

    ss >> calendar.monday >> calendar.tuesday >> calendar.wednesday >> calendar.thursday
       >> calendar.friday >> calendar.saturday >> calendar.sunday;

    std::getline(ss, calendar.start_date, ',');
    std::getline(ss, calendar.end_date);

    calendars[calendar.service_id] = calendar;
  }
  return calendars;
}

std::unordered_map<int, Route> readRoutes(const std::string &filePath) {
  std::unordered_map<int, Route> routes;
  std::ifstream file(filePath);
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Route route;

    std::string route_id_str;
    std::getline(ss, route_id_str, ',');
    route.route_id = std::stoi(route_id_str);

    std::string agency_id_str;
    std::getline(ss, agency_id_str, ',');
    route.agency_id = std::stoi(agency_id_str);

    std::getline(ss, route.route_short_name, ',');
    std::getline(ss, route.route_long_name, ',');

    std::string route_type_str;
    std::getline(ss, route_type_str);
    route.route_type = std::stoi(route_type_str);

    routes[route.route_id] = route;

  }
  return routes;
}

std::unordered_map<int, Stop> readStops(const std::string &filePath) {
  std::unordered_map<int, Stop> stops;
  std::ifstream file(filePath);
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Stop stop;

    std::string stop_id_str;
    std::getline(ss, stop_id_str, ',');
    stop.stop_id = std::stoi(stop_id_str);

    std::getline(ss, stop.stop_name, ',');

    std::string lat_str;
    std::getline(ss, lat_str, ',');

    std::string lon_str;
    std::getline(ss, lon_str);

    stop.coordinates.lat = std::stod(lat_str);
    stop.coordinates.lon = std::stod(lon_str);

    stops[stop.stop_id] = stop;
  }
  return stops;
}

std::unordered_map<int, Trip> readTrips(const std::string &filePath) {
  std::unordered_map<int, Trip> trips;
  std::ifstream file(filePath);
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Trip trip;

    std::string route_id_str;
    std::getline(ss, route_id_str, ',');
    trip.route_id = std::stoi(route_id_str);

    std::string service_id_str;
    std::getline(ss, service_id_str, ',');
    trip.service_id = std::stoi(service_id_str);

    std::string trip_id_str;
    std::getline(ss, trip_id_str, ',');
    trip.trip_id = std::stoi(trip_id_str);

    trips[trip.trip_id] = trip;
  }
  return trips;
}

std::vector<StopTime> readStopTimes(const std::string &filePath) {
  std::vector<StopTime> stop_times;
  std::ifstream file(filePath);
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    StopTime stop_time;

    std::string trip_id_str;
    std::getline(ss, trip_id_str, ',');
    stop_time.trip_id = std::stoi(trip_id_str);

    std::getline(ss, stop_time.arrival_time, ',');
    std::getline(ss, stop_time.departure_time, ',');

    std::string stop_id_str;
    std::getline(ss, stop_id_str, ',');
    stop_time.stop_id = std::stoi(stop_id_str);

    ss >> stop_time.stop_sequence;
    stop_times.push_back(stop_time);
    // TODO: primary key is trip_id, stop_sequence
  }
  return stop_times;
}

void cleanLine(std::string &line) {
  line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
  line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
}