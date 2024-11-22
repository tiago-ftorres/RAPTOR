//
// Created by maria on 11/20/2024.
//

#include "Stop.h"

void Stop::addStopTime(StopTime *stopTime) {
  stop_times.push_back(stopTime);
}

void Stop::addRouteKey(const std::pair<std::string, std::string>& route_key) {
  routes_keys.push_back(route_key);
}

void Stop::addFootpath(const std::string &other_id, int &duration) {
  footpaths[other_id] = {other_id, duration};
}

const std::vector<StopTime *> &Stop::getStopTimes() const {
  return stop_times;
}

const std::vector<std::pair<std::string, std::string>> &Stop::getRouteKeys() const {
  return routes_keys;
}

const std::unordered_map<std::string, Footpath> &Stop::getFootpaths() const {
  return footpaths;
}

void Stop::sortStopTimes() {
  std::sort(stop_times.begin(), stop_times.end(), [&](const StopTime *a, const StopTime *b) {
    return Utils::timeToSeconds(a->getField("departure_time")) < Utils::timeToSeconds(b->getField("departure_time"));
  });
}

