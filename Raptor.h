//
// Created by maria on 10/28/2024.
//

#ifndef RAPTOR_RAPTOR_H
#define RAPTOR_RAPTOR_H

#include <vector>
#include <queue>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include "Parse.h"
#include "Utils.h"

const int INF = std::numeric_limits<int>::max();

class Raptor {
public:
  Raptor(const std::unordered_map<int, Stop>& stops,
         const std::unordered_map<int, Route>& routes,
         const std::unordered_map<int, Trip>& trips,
         const std::vector<StopTime>& stop_times);

  // Returns earliest arrival time at destination stop
  int findRoute(const Query& query);

private:
  void initializeData();

  // TODO
  std::vector<int> getRoutesServingStop(const int& stop_id);
  std::vector<StopTime> getStopTimesForRoute(const int& route_id);
  int getArrivalTimeForTrip(const int& trip_id, const int& stop_id);
  int getDepartureTimeForTrip(const int& trip_id, const int& stop_id);
  int getNextTrip(const int& route_id, const int& stop_id);

  std::unordered_map<int, Stop> stops_;
  std::unordered_map<int, Route> routes_;
  std::unordered_map<int, Trip> trips_;
  std::vector<StopTime> stop_times_;
};

#endif //RAPTOR_RAPTOR_H
