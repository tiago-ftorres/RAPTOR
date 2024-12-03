//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_STOP_H
#define RAPTOR_STOP_H

#include "GTFSObject.h"
#include "../DataStructures.h"

class Stop : public GTFSObject {
public:
  void addStopTimeKey(const std::pair<std::string, std::string> &stop_time_key); // key is (trip_id, stop_id)
  void addRouteKey(const std::pair<std::string, std::string> &route_key);

  void addFootpath(const std::string &other_id, int &duration);

  const std::vector<std::pair<std::string, std::string>> &getStopTimesKeys() const;

  const std::vector<std::pair<std::string, std::string>> &getRouteKeys() const;

  const std::unordered_map<std::string, Footpath> &getFootpaths() const;

  void sortStopTimes(const std::function<bool(const std::pair<std::string, std::string> &,
                                              const std::pair<std::string, std::string> &)> &comparator);

private:
  std::vector<std::pair<std::string, std::string>> stop_times_keys; // Sorted by earliest to latest departure time
  std::vector<std::pair<std::string, std::string>> routes_keys; // (route_id, direction_id)
  std::unordered_map<std::string, Footpath> footpaths;

};


#endif //RAPTOR_STOP_H
