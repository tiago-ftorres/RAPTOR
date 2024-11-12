
#include "Parser.h"

std::unordered_map<std::string, Agency> Parser::readAgencies() const {
  std::unordered_map<std::string, Agency> agencies;
  std::ifstream file(inputDirectory + "/agency.txt");
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Agency agency;

    std::getline(ss, agency.agency_id, ',');
    std::getline(ss, agency.agency_name, ',');

    agencies[agency.agency_id] = agency;
  }
  return agencies;
}

std::unordered_map<std::string, Calendar> Parser::readCalendars() const {
  std::unordered_map<std::string, Calendar> calendars;
  std::ifstream file(inputDirectory + "/calendar.txt");
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Calendar calendar;

    std::getline(ss, calendar.service_id, ',');

    ss >> calendar.monday >> calendar.tuesday >> calendar.wednesday >> calendar.thursday
       >> calendar.friday >> calendar.saturday >> calendar.sunday;

    std::getline(ss, calendar.start_date, ',');
    std::getline(ss, calendar.end_date);

    calendars[calendar.service_id] = calendar;
  }
  return calendars;
}

std::unordered_map<std::string, Route> Parser::readRoutes() const {
  std::unordered_map<std::string, Route> routes;
  std::ifstream file(inputDirectory + "routes.txt");
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Route route;

    std::getline(ss, route.route_id, ',');
    std::getline(ss, route.agency_id, ',');
    std::getline(ss, route.route_short_name, ',');
    std::getline(ss, route.route_long_name, ',');
    std::getline(ss, route.route_desc, ',');

    std::string route_type_str;
    std::getline(ss, route_type_str);
    route.route_type = std::stoi(route_type_str);

    routes[route.route_id] = route;

  }
  return routes;
}

std::unordered_map<std::string, Stop> Parser::readStops() const {
  std::unordered_map<std::string, Stop> stops;
  std::ifstream file(inputDirectory + "stops.txt");
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Stop stop;

    std::getline(ss, stop.stop_id, ',');
    std::getline(ss, stop.stop_code, ',');
    std::getline(ss, stop.stop_name, ',');
    std::getline(ss, stop.stop_desc, ',');

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

std::unordered_map<std::string, Trip> Parser::readTrips() const {
  std::unordered_map<std::string, Trip> trips;
  std::ifstream file(inputDirectory + "trips.txt");
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    Trip trip;

    std::getline(ss, trip.route_id, ',');
    std::getline(ss, trip.service_id, ',');
    std::getline(ss, trip.trip_id, ',');

    trips[trip.trip_id] = trip;
  }
  return trips;
}

std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> Parser::readStopTimes() const {
  std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> stop_times; // key is (trip_id, stop_id)
  std::ifstream file(inputDirectory + "stop_times.txt");
  std::string line;
  std::getline(file, line);  // Skip header

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) {
      continue;
    }

    std::stringstream ss(line);
    StopTime stop_time;

    std::getline(ss, stop_time.trip_id, ',');

    std::getline(ss, stop_time.arrival_time, ',');
    std::getline(ss, stop_time.departure_time, ',');

    std::getline(ss, stop_time.stop_id, ',');

    ss >> stop_time.stop_sequence;
    stop_times[{stop_time.trip_id, stop_time.stop_id}] = stop_time;
  }
  return stop_times;
}

void Parser::cleanLine(std::string &line) {
  line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
  line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
}
