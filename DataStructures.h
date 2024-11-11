#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct Agency {
  int agency_id;
  std::string agency_name;
};

struct Calendar {
  int service_id;
  bool monday, tuesday, wednesday, thursday, friday, saturday, sunday;
  std::string start_date, end_date;
};

struct StopTime {
  int trip_id;
  std::string arrival_time;
  std::string departure_time;
  int stop_id;
  int stop_sequence;
};

struct Coordinates {
  double lat;
  double lon;
};

struct Footpath {
  int dest_id;
  int duration;
};

struct Stop {
  int stop_id;
  std::string stop_name;
  Coordinates coordinates;
  std::vector<StopTime*> stop_times; // Ordered by earliest to latest departure time
  std::vector<int> routes_ids;
  std::unordered_map<int, Footpath> footpaths;

  // Stop() : stop_id(0), stop_name(""), coordinates(), footpaths() {}
};

struct Trip {
  int route_id;
  int service_id;
  int trip_id;
  std::vector<StopTime*> stop_times;
};

struct Route {
  int route_id;
  int agency_id;
  std::string route_short_name;
  std::string route_long_name;
  int route_type;

  std::vector<Trip*> trips; // Trips that follow this route, ordered by earliest to latest arrival time
  std::vector<Stop*> stops; // Stops that compose this route
};

struct Query {
  int source_id;
  int target_id;
  std::string departure_time;
};

struct StopInfo {
  int min_arrival_time;
  int parent_trip_id; // if footpath, parent_trip_id = -1
  int parent_stop_id; // if first stop, parent_stop_id = -1
};

struct JourneyStep {
  int trip_id; // if footpath, trip_id = -1
  int stop_src_id;
  int stop_dest_id;
  int departure_time;
  int arrival_time;
};

#endif //DATASTRUCTURES_H
