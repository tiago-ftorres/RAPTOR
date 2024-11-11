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
         const std::unordered_map<std::pair<int, int>, StopTime, pair_hash>& stop_times);

  // Returns all Pareto-optimal journeys
  std::vector<std::vector<JourneyStep>> findRoute(const Query& query);

  // Reconstruct all journeys
  std::vector<std::vector<JourneyStep>> reconstructJourneys(const Query &query);

  // Prints minimal arrival times for each stop, at each round
  void printMinArrivalTimes();

private:
  void initializeData();

  std::unordered_map<int, Stop> stops_;
  std::unordered_map<int, Route> routes_;
  std::unordered_map<int, Trip> trips_;
//  std::vector<StopTime> stop_times_;
  std::unordered_map<std::pair<int, int>, StopTime, pair_hash> stop_times_; // key is (trip_id, stop_id)
  std::unordered_map<int, std::vector<StopInfo>> min_arrival_time;
  int k;

};

#endif //RAPTOR_RAPTOR_H
