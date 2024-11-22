#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <algorithm>
#include "GTFSObjects/GTFSObject.h"

static constexpr int INF = std::numeric_limits<int>::max();

struct Footpath {
  std::string dest_id;
  int duration;
};

struct Query {
  std::string source_id;
  std::string target_id;
  std::string departure_time;
};

struct StopInfo {
  int min_arrival_time;
  std::string parent_trip_id; // if footpath, parent_trip_id = -1
  std::string parent_stop_id; // if first stop, parent_stop_id = -1
};

struct JourneyStep {
  std::string trip_id; // if footpath, trip_id = -1
  std::string stop_src_id;
  std::string stop_dest_id;
//  Stop* src_stop;
//  Stop* dest_stop;

  int departure_time;
  int arrival_time;
};

struct pair_hash {
  std::size_t operator()(const std::pair<std::string, std::string>& pair) const {
    return std::hash<std::string>()(pair.first) ^ std::hash<std::string>()(pair.second);
  }
};

struct nested_pair_hash {
  std::size_t operator()(const std::pair<std::pair<std::string, std::string>, std::string>& nested_pair) const {
    std::size_t hash1 = pair_hash{}(nested_pair.first);  // Hash of internal part
    std::size_t hash2 = std::hash<std::string>{}(nested_pair.second);
    return hash1 ^ (hash2 << 1);
  }
};

#endif //DATASTRUCTURES_H
