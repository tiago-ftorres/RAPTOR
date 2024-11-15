//
// Created by maria on 10/28/2024.
//

#ifndef RAPTOR_RAPTOR_H
#define RAPTOR_RAPTOR_H

#include <map>
#include "Parser.h"
#include "Utils.h"

class Raptor {
  public:
    Raptor(const std::unordered_map<std::string, Stop>& stops,
           const std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash>& routes,
           const std::unordered_map<std::string, Trip>& trips,
           const std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash>& stop_times);

    // Returns all Pareto-optimal journeys
    std::vector<std::vector<JourneyStep>> findJourneys(const Query& query);

    // Reconstruct all journeys
    std::vector<std::vector<JourneyStep>> reconstructJourneys(const Query &query);

    // Prints minimal arrival times for each stop, at each round
    void showMinArrivalTimes();

  private:
    void initializeData();

    std::unordered_map<std::string, Stop> stops_;
    std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> routes_;
    std::unordered_map<std::string, Trip> trips_;
    std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> stop_times_; // key is (trip_id, stop_id)
    std::map<std::string, std::vector<StopInfo>> min_arrival_time; // each stop_id has a vector of min_arrival time for each k
    int k;

};

#endif //RAPTOR_RAPTOR_H
