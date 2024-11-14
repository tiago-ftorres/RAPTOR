
#include "Parser.h"

Parser::Parser(std::string directory) : inputDirectory(std::move(directory)) {
  parseAgencies();
  parseCalendars();
  parseTrips();
  parseRoutes();
  parseStops();
  parseStopTimes();

  std::cout << "Agencies numbers: " << agencies_.size() << std::endl;
  std::cout << "Calendars number: " << calendars_.size() << std::endl;
  std::cout << "Trips number: " << trips_.size() << std::endl;
  std::cout << "Routes number: " << routes_.size() << std::endl;
  std::cout << "Stops number: " << stops_.size() << std::endl;
  std::cout << "Stop Times number: " << stop_times_.size() << std::endl;

}

void Parser::parseAgencies() {
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

    agencies_[agency.agency_id] = agency;
  }
}

void Parser::parseCalendars() {
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

    calendars_[calendar.service_id] = calendar;
  }
}

void Parser::parseTrips() {
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
    std::getline(ss, trip.head_sign, ',');
    std::getline(ss, trip.direction_id, ',');

    trips_[trip.trip_id] = trip;
    routes_[std::make_pair(trip.route_id, trip.direction_id)]; // Create entry
  }
}

void Parser::parseRoutes() {
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

    // Iterate through all existing (route_id, direction_id) pairs in routes_
    for (auto& [key, r] : routes_) {
      // if key.route_id == route.route_id
      if (key.first == route.route_id) {
        routes_[key]=route;
      }
    }

  }
}

// TODO: for all, fill fields based on header
void Parser::parseStops() {
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

    stops_[stop.stop_id] = stop;
  }
}

void Parser::parseStopTimes() {
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
    stop_times_[{stop_time.trip_id, stop_time.stop_id}] = stop_time;
  }
}

void Parser::cleanLine(std::string &line) {
  line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
  line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
}

std::unordered_map<std::string, Stop> Parser::getStops() {
  return stops_;
}

std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> Parser::getRoutes() {
  return routes_;
}

std::unordered_map<std::string, Trip> Parser::getTrips() {
  return trips_;
}

std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> Parser::getStopTimes() {
  return stop_times_;
}
