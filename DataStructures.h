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

struct Route {
  int route_id;
  int agency_id;
  std::string route_short_name;
  std::string route_long_name;
  int route_type;
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
  std::unordered_map<int, Footpath> footpaths;

 // Stop() : stop_id(0), stop_name(""), coordinates(), footpaths() {}
};

struct Trip {
  int route_id;
  int service_id;
  int trip_id;
};

struct Query {
  int source_id;
  int target_id;
  std::string departure_time;
};

#endif //DATASTRUCTURES_H
