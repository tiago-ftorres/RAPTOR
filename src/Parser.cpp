
#include "Parser.h"

Parser::Parser(std::string directory) : inputDirectory(std::move(directory)) {
  std::cout << "Parsing GTFS data from " << inputDirectory << "..." << std::endl;

  parseAgencies();
  std::cout << agencies_.size() << " agencies, ";

  parseCalendars();
  std::cout << calendars_.size() << " calendars, ";

  parseTrips();
  std::cout << trips_.size() << " trips, ";

  parseRoutes();
  std::cout << routes_.size() << " routes, ";

  parseStops();
  std::cout << stops_.size() << " stops and ";

  parseStopTimes();
  std::cout << stop_times_.size() << " stop times parsed. ";

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

    // If there is only one agency, agency_id field is optional
    if (!route.hasField("agency_id"))
      route.setField("agency_id", agencies_.begin()->second.getField("agency_id"));

    // Iterate through all existing (route_id, direction_id) pairs in routes_
    for (auto &[key, r]: routes_) {
      // if key.route_id == route.route_id
      auto [route_id, direction_id] = key;
      if (route_id == route.getField("route_id")) {
        route.setField("direction_id", direction_id);
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

    stop_time.setArrivalSeconds(Utils::timeToSeconds(stop_time.getField("arrival_time")));
    stop_time.setDepartureSeconds(Utils::timeToSeconds(stop_time.getField("departure_time")));

    stop_times_[{stop_time.getField("trip_id"), stop_time.getField("stop_id")}] = stop_time;
  }
}

void Parser::associateData() {
  // Associate stop_times to trips
  for (const auto &[key, stop_time]: stop_times_) {
    const auto &[trip_id, stop_id] = key;
    trips_[trip_id].addStopTimeKey(key);
  }

  // Sort each trip's stop_times
  for (auto &[id, trip]: trips_)
    trip.sortStopTimes([&](const std::pair<std::string, std::string> &a, const std::pair<std::string, std::string> &b) {
      const StopTime &stopTimeA = stop_times_.at(a);
      const StopTime &stopTimeB = stop_times_.at(b);

      int seqA = std::stoi(stopTimeA.getField("stop_sequence"));
      int seqB = std::stoi(stopTimeB.getField("stop_sequence"));

      return seqA < seqB;
    });

  // Associate trips to routes
  for (auto &[id, trip]: trips_) {
    auto route_key = std::make_pair(trip.getField("route_id"), trip.getField("direction_id"));
    routes_[route_key].addTripId(id);
  }

  // Sort each route's trip by earliest to latest arrival time
  for (auto &[id, route]: routes_)
    route.sortTrips([&](const std::string &a, const std::string &b) {
      const Trip &tripA = trips_.at(a);
      const Trip &tripB = trips_.at(b);

      const StopTime &stopTimeA = stop_times_.at(tripA.getStopTimesKeys().front());
      const StopTime &stopTimeB = stop_times_.at(tripB.getStopTimesKeys().front());

      int timeA = Utils::timeToSeconds(stopTimeA.getField("arrival_time"));
      int timeB = Utils::timeToSeconds(stopTimeB.getField("arrival_time"));

      return timeA < timeB;
    });

  // Associate stops to routes
  for (auto &[key, route]: routes_) {
    // If there is no trip associated to this route, skip the route
    if (route.getTripsIds().empty()) continue;

    // Find the route's trip that has the most stops
    Trip &largest_trip = trips_[route.getTripsIds()[0]];
    for (size_t i = 1; i < route.getTripsIds().size(); i++) {
      Trip &trip = trips_[route.getTripsIds()[i]];
      if (trip.getStopTimesKeys().size() > largest_trip.getStopTimesKeys().size()) {
        largest_trip = trip;
      }
    }

    // A route stops' order will be the same as the routes' largest_trip stops' order, because it has the most stops
    for (const auto &[trip_id, stop_id]: largest_trip.getStopTimesKeys()) {
      route.addStopId(stop_id);
    }
  }

  for (auto &[key, stop_time]: stop_times_) {
    auto &[trip_id, stop_id] = key;
    // Associate stop_times to stops
    stops_[stop_id].addStopTimeKey(key);

    // Associate routes to stops
    stops_[stop_id].addRouteKey(
            std::make_pair(trips_[trip_id].getField("route_id"), trips_[trip_id].getField("direction_id")));
  }

  // Sort each stop's stop_times by earliest to latest departure time
  for (auto &[id, stop]: stops_)
    stop.sortStopTimes([&](const std::pair<std::string, std::string> &a, const std::pair<std::string, std::string> &b) {
      const StopTime &stopTimeA = stop_times_.at(a);
      const StopTime &stopTimeB = stop_times_.at(b);

      int timeA = Utils::timeToSeconds(stopTimeA.getField("departure_time"));
      int timeB = Utils::timeToSeconds(stopTimeB.getField("departure_time"));

      return timeA < timeB;
    });


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

