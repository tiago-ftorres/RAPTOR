
#include "Parser.h"

Parser::Parser(std::string directory) : inputDirectory(std::move(directory)) {
  parseAgencies();
  std::cout << "Agencies numbers: " << agencies_.size() << std::endl;

  parseCalendars();
  std::cout << "Calendars number: " << calendars_.size() << std::endl;

  parseTrips();
  std::cout << "Trips number: " << trips_.size() << std::endl;

  parseRoutes();
  std::cout << "Routes number: " << routes_.size() << std::endl;

  parseStops();
  std::cout << "Stops number: " << stops_.size() << std::endl;

  parseStopTimes();
  std::cout << "Stop Times number: " << stop_times_.size() << std::endl;

  associateData();
  std::cout << "Data associated." << std::endl;

}

void Parser::parseAgencies() {
  std::ifstream file(inputDirectory + "/agency.txt");

  if (!file.is_open())
    throw std::runtime_error("Could not open agency.txt");

  std::string line;
  std::getline(file, line);
  std::vector<std::string> fields = Utils::split(line, ',');

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) continue;

    std::vector<std::string> tokens = Utils::split(line, ',');

    if (tokens.size() != fields.size())
      throw std::runtime_error("Mismatched number of tokens and fields");

    Agency agency;

    for (size_t i = 0; i < fields.size(); ++i)
      agency.setField(fields[i], tokens[i]);

    agencies_[agency.getField("agency_id")] = agency;
  }
}

void Parser::parseCalendars() {
  std::ifstream file(inputDirectory + "/calendar.txt");
  if (!file.is_open())
    throw std::runtime_error("Could not open calendar.txt");

  std::string line;
  std::getline(file, line);
  std::vector<std::string> fields = Utils::split(line, ',');

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) continue;

    std::vector<std::string> tokens = Utils::split(line, ',');

    if (tokens.size() != fields.size())
      throw std::runtime_error("Mismatched number of tokens and fields");


    Calendar calendar;

    for (size_t i = 0; i < fields.size(); ++i)
      calendar.setField(fields[i], tokens[i]);

    calendars_[calendar.getField("service_id")] = calendar;
  }
}

void Parser::parseTrips() {
  std::ifstream file(inputDirectory + "trips.txt");

  if (!file.is_open())
    throw std::runtime_error("Could not open trips.txt");

  std::string line;
  std::getline(file, line);
  std::vector<std::string> fields = Utils::split(line, ',');

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) continue;

    std::vector<std::string> tokens = Utils::split(line, ',');

    if (tokens.size() != fields.size())
      throw std::runtime_error("Mismatched number of tokens and fields");

    Trip trip;
    for (size_t i = 0; i < fields.size(); ++i)
      trip.setField(fields[i], tokens[i]);

    trips_[trip.getField("trip_id")] = trip;
    routes_[std::make_pair(trip.getField("route_id"), trip.getField("direction_id"))]; // Create entry
  }
}

void Parser::parseRoutes() {
  std::ifstream file(inputDirectory + "routes.txt");

  if (!file.is_open())
    throw std::runtime_error("Could not open routes.txt");

  std::string line;
  std::getline(file, line);
  std::vector<std::string> fields = Utils::split(line, ',');

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) continue;

    std::vector<std::string> tokens = Utils::split(line, ',');

    if (tokens.size() != fields.size())
      throw std::runtime_error("Mismatched number of tokens and fields");

    Route route;
    for (size_t i = 0; i < fields.size(); ++i)
      route.setField(fields[i], tokens[i]);

    // Iterate through all existing (route_id, direction_id) pairs in routes_
    for (auto &[key, r]: routes_) {
      // if key.route_id == route.route_id
      if (key.first == route.getField("route_id")) {
        route.setField("direction_id", key.second);
        routes_[key] = route;
      }
    }

  }
}

void Parser::parseStops() {
  std::ifstream file(inputDirectory + "stops.txt");

  if (!file.is_open())
    throw std::runtime_error("Could not open stops.txt");

  std::string line;
  std::getline(file, line);
  std::vector<std::string> fields = Utils::split(line, ',');

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) continue;

    std::vector<std::string> tokens = Utils::split(line, ',');

    if (tokens.size() != fields.size())
      throw std::runtime_error("Mismatched number of tokens and fields");

    Stop stop;
    for (size_t i = 0; i < fields.size(); ++i)
      stop.setField(fields[i], tokens[i]);

    stops_[stop.getField("stop_id")] = stop;
  }
}

void Parser::parseStopTimes() {
  std::ifstream file(inputDirectory + "stop_times.txt");
  if (!file.is_open())
    throw std::runtime_error("Could not open stop_times.txt");

  std::string line;
  std::getline(file, line);
  std::vector<std::string> fields = Utils::split(line, ',');

  while (std::getline(file, line)) {
    cleanLine(line);

    if (line.empty()) continue;

    std::vector<std::string> tokens = Utils::split(line, ',');

    if (tokens.size() != fields.size())
      throw std::runtime_error("Mismatched number of tokens and fields");

    StopTime stop_time;

    for (size_t i = 0; i < fields.size(); ++i)
      stop_time.setField(fields[i], tokens[i]);

    stop_times_[{stop_time.getField("trip_id"), stop_time.getField("stop_id")}] = stop_time;
  }
}

void Parser::associateData() {
  // Initialize footpaths
  std::cout << "Initializing footpaths..." << std::endl;
  auto start_time = std::chrono::high_resolution_clock::now();

  // Avoid duplicating calculations for both sides
  for (auto it1 = stops_.begin(); it1 != stops_.end(); ++it1) {
    const std::string &id1 = it1->first;
    Stop &stop1 = it1->second;

    // Start the inner loop from the next element
    for (auto it2 = std::next(it1); it2 != stops_.end(); ++it2) {
      const std::string &id2 = it2->first;
      Stop &stop2 = it2->second;

      // Calculate duration between the two stops
      int duration = Utils::getDuration(
              stop1.getField("stop_lat"), stop1.getField("stop_lon"),
              stop2.getField("stop_lat"), stop2.getField("stop_lon"));

      // Add footpaths in both directions
      stop1.addFootpath(id2, duration);
      stop2.addFootpath(id1, duration);
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

  std::cout << "Footpaths initialized in " << duration << " ms ("
            << duration / 1000 << " seconds)." << std::endl;

  // Associate stop_times to trips
  for (auto &[key, stop_time]: stop_times_) {
    auto &[trip_id, stop_id] = key;
    trips_[trip_id].addStopTime(&stop_time);
  }

  // Sort each trip's stop_times
  for (auto &[id, trip]: trips_)
    trip.sortStopTimes();

  // Associate trips to routes
  for (auto &[id, trip]: trips_) {
    auto route_key = std::make_pair(trip.getField("route_id"), trip.getField("direction_id"));
    routes_[route_key].addTrip(&trip);
  }

  // Sort each route's trip by earliest to latest arrival time
  for (auto &[id, route]: routes_)
    route.sortTrips();

  // Associate stops to routes
  for (auto &[key, route]: routes_) {
    // If there is no trip associated to this route, skip the route
    if (route.getTrips().empty()) continue;

    // Find the route's trip that has the most stops
    Trip *largest_trip = route.getTrips()[0];
    for (size_t i = 1; i < route.getTrips().size(); i++) {
      Trip *trip = route.getTrips()[i];
      if (trip->getStopTimes().size() > largest_trip->getStopTimes().size()) {
        largest_trip = trip;
      }
    }

    // A route stops' order will be the same as the routes' largest_trip stops' order, because it has the most stops
    for (const auto &stop_time: largest_trip->getStopTimes()) {
      route.addStop(&stops_[stop_time->getField("stop_id")]);
    }
  }

  for (auto &[key, stop_time]: stop_times_) {
    auto &[trip_id, stop_id] = key;
    // Associate stop_times to stops
    stops_[stop_id].addStopTime(&stop_time);

    // Associate routes to stops
    stops_[stop_id].addRouteKey(
            std::make_pair(trips_[trip_id].getField("route_id"), trips_[trip_id].getField("direction_id")));
  }

  // Sort each stop's stop_times by earliest to latest departure time
  for (auto &[id, stop]: stops_)
    stop.sortStopTimes();

}

void Parser::cleanLine(std::string &line) {
  line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
  line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
}

std::unordered_map<std::string, Agency> Parser::getAgencies() {
  return agencies_;
}

std::unordered_map<std::string, Calendar> Parser::getCalendars() {
  return calendars_;
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

