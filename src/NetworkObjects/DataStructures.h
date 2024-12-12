#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "../DateTime.h"

class Stop;

struct Query {
  std::string source_id;
  std::string target_id;
  Date date;
  Time departure_time;
};

struct StopInfo {
  std::optional<int> arrival_seconds; // if unreachable, will be std::nullopt
  std::optional<std::string> parent_trip_id; // if footpath, will be std::nullopt
  std::optional<std::string> parent_stop_id; // if first stop, will be std::nullopt
  std::optional<Day> day; // if unreachable, will be std::nullopt
};

struct JourneyStep {
  std::optional<std::string> trip_id; // if footpath, will be std::nullopt
  std::optional<std::string> agency_name; // if footpath, will be std::nullopt
  Stop *src_stop{};
  Stop *dest_stop{};

  int departure_secs{};
  Day day{};
  int duration{};
  int arrival_secs{};
};

struct Journey {
  std::vector<JourneyStep> steps;
  int departure_secs;
  Day departure_day;

  int arrival_secs;
  Day arrival_day;

  int duration;
};

struct pair_hash {
  std::size_t operator()(const std::pair<std::string, std::string> &pair) const {
    return std::hash<std::string>()(pair.first) ^ std::hash<std::string>()(pair.second);
  }
};

struct nested_pair_hash {
  std::size_t operator()(const std::pair<std::pair<std::string, std::string>, std::string> &nested_pair) const {
    std::size_t hash1 = pair_hash{}(nested_pair.first);  // Hash of internal part
    std::size_t hash2 = std::hash<std::string>{}(nested_pair.second);
    return hash1 ^ (hash2 << 1);
  }
};

#endif //DATASTRUCTURES_H
