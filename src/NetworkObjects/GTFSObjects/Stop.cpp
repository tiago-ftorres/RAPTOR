//
// Created by maria on 11/20/2024.
//

#include "Stop.h"

void Stop::addStopTimeKey(const std::pair<std::string, std::string> &stop_time_key) {
  stop_times_keys.push_back(stop_time_key);
}

void Stop::addRouteKey(const std::pair<std::string, std::string> &route_key) {
  routes_keys.insert(route_key);
}

void Stop::addFootpath(const std::string &other_id, int &duration) {
  footpaths[other_id] = {other_id, duration};
}

const std::vector<std::pair<std::string, std::string>> &Stop::getStopTimesKeys() const {
  return stop_times_keys;
}

const std::unordered_set<std::pair<std::string, std::string>, pair_hash> &Stop::getRouteKeys() const {
  return routes_keys;
}

const std::unordered_map<std::string, Footpath> &Stop::getFootpaths() const {
  return footpaths;
}
// TODO: map with customized comparator instead of vector
void Stop::sortStopTimes(const std::function<bool(const std::pair<std::string, std::string>&,
                                                  const std::pair<std::string, std::string>&)>& comparator) {
  std::sort(stop_times_keys.begin(), stop_times_keys.end(), comparator);
}

