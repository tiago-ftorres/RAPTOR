//
// Created by maria on 10/28/2024.
//

#ifndef RAPTOR_RAPTOR_H
#define RAPTOR_RAPTOR_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>  // for setw
#include "Parser.h"
#include "Utils.h"

class Raptor {
public:

  Raptor(const std::unordered_map<std::string, Agency> &agencies_,
         const std::unordered_map<std::string, Calendar> &calendars_,
         const std::unordered_map<std::string, Stop> &stops,
         const std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> &routes,
         const std::unordered_map<std::string, Trip> &trips,
         const std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> &stop_times);

  void setQuery(const Query &query);

  // Returns all Pareto-optimal journeys
  std::vector<std::vector<JourneyStep>> findJourneys();

  // Prints a journey`s steps
  static void showJourney(const std::vector<JourneyStep> &journey);

  const std::unordered_map<std::string, Stop> &getStops() const;

private:

  std::unordered_map<std::string, Agency> agencies_;
  std::unordered_map<std::string, Calendar> calendars_;
  std::unordered_map<std::string, Stop> stops_;
  std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> routes_;
  std::unordered_map<std::string, Trip> trips_;
  std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> stop_times_; // key is (trip_id, stop_id)

  Query query_;
  std::unordered_map<std::string, std::vector<StopInfo>> arrivals_; // each stop_id has a vector of StopInfo (arrival time) for each k
  std::unordered_set<std::string> prev_marked_stops;
  std::unordered_set<std::string> marked_stops;
  int k;

  void initializeFootpaths();

  void initializeAlgorithm();

  void setMinArrivalTime(const std::string &stop_id, StopInfo stop_info);

  void fillActiveTrips(Day day);

  std::unordered_set<std::pair<std::pair<std::string, std::string>, std::string>, nested_pair_hash>
  accumulateRoutesServingStops();

  void traverseRoutes(
          std::unordered_set<std::pair<std::pair<std::string, std::string>, std::string>, nested_pair_hash> routes_stops_set);

  std::optional<std::string>
  findEarliestTrip(const std::string &pi_stop_id, const std::pair<std::string, std::string> &route_key);

  bool
  isValidTrip(const std::pair<std::string, std::string> &route_key, const StopTime &stop_time);

  static bool isServiceActive(const Calendar &calendar, const Date &date) ;

  void traverseTrip(std::string &et_id, std::string &pi_stop_id);

  void handleFootpaths();

  static bool isFootpath(const StopInfo &stop_info);

  static bool arrivesEarlier(int secondsA, std::optional<int> secondsB);

  // Reconstruct journey at current round k
  std::vector<JourneyStep> reconstructJourney();

  bool isValidJourney(std::vector<JourneyStep> journey) const;

  bool improvesArrivalTime(int arrival, const std::string& dest_id);
};

#endif //RAPTOR_RAPTOR_H
