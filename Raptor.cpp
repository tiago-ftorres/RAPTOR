#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "Raptor.h"

Raptor::Raptor(const std::unordered_map<int, Stop>& stops,
               const std::unordered_map<int, Route>& routes,
               const std::unordered_map<int, Trip>& trips,
               const std::vector<StopTime>& stop_times) // TODO: primary key is trip_id, stop_sequence
                : stops_(stops), routes_(routes), trips_(trips), stop_times_(stop_times) {
  initializeData();
}

void Raptor::initializeData() {
  // TODO: initialize other things?
  // Initialize footpaths
  for (auto& [id, stop] : stops_) {
    for (const auto& [other_id, other_stop]: stops_) {
      if (id != other_id) {
        stop.footpaths[other_id] = {other_id, getDuration(stop.coordinates, other_stop.coordinates)};
      }
    }
  }
}

int Raptor::findRoute(const Query &query) {
  //TODO: std::vector<Trip> journey;

  std::unordered_map<int, std::vector<int>> min_arrival_time;
  std::unordered_set<int> marked_stops;

  // Initialization of the algorithm
  for (const auto& [id, stop]: stops_) {
    min_arrival_time[id] = std::vector<int>(2, INF);
  }

  min_arrival_time[query.source_id][0] = timeToSeconds(query.departure_time);
  marked_stops.insert(query.source_id);

  int k = 1;
  while (true) {
    // Accumulate routes serving marked stops from previous round
    std::queue<int> route_queue;

    // For each marked stop p
    for (const auto &stop_id: marked_stops) {
      // For each route r serving p
      for (const auto &route: getRoutesServingStop(stop_id)) {

        // TODO:
        // if (r, p') E Q for some stop p'
        //  if p comes before p' in r
        //    Substitute (r, p') by (r, p) in Q
        //  else
        //    Add(r, p) to Q

        route_queue.push(route);
      }
      // Unmark p
      marked_stops.erase(stop_id);
    }

    // Traverse each route
    while (!route_queue.empty()) {
      auto route = route_queue.front();
      route_queue.pop();
      int current_trip;

      for (const auto &stop_time: getStopTimesForRoute(route)) { // TODO: beginning with p
        const auto &stop_id = stop_time.stop_id;
        int arrival_time = getArrivalTimeForTrip(current_trip, stop_id);

        // If arrival time can be improved
        if (arrival_time < std::min(min_arrival_time[stop_id][k], min_arrival_time[query.target_id][k])) {
          min_arrival_time[stop_id][k] = arrival_time;
          marked_stops.insert(stop_id);
        }

        // Can we catch an earlier trip at pi?
        if (min_arrival_time[stop_id][k - 1] <= getDepartureTimeForTrip(current_trip, stop_id)) {
          current_trip = getNextTrip(route, stop_id);
        }
      }
    }

    // Look for footh-paths
    // For each marked stop p
    for (const auto& stop_id: marked_stops) {
      // For each footh-path (p, p') E F
      for (const auto& [dest_id, footpath]: stops_[stop_id].footpaths) {
        int new_arrival = min_arrival_time[stop_id][k] + footpath.duration;
        if (new_arrival < min_arrival_time[dest_id][k]) {
          min_arrival_time[dest_id][k] = new_arrival;
          marked_stops.insert(dest_id);
        }
      }
    }

    // Stopping criterion
    // if no stops are marked, then stop
    if (marked_stops.empty()) {
      break;
    }

    k++;
  }

  return min_arrival_time[query.target_id][k];
}


std::vector<int> Raptor::getRoutesServingStop(const int &stop_id) {
  std::vector<int> routes;

  return routes;
}

std::vector<StopTime> Raptor::getStopTimesForRoute(const int &route_id) {
  std::vector<StopTime> stop_times;

  return stop_times;
}

int Raptor::getArrivalTimeForTrip(const int &trip_id, const int &stop_id) {
  int arrival_time = 0;

  return arrival_time;
}

int Raptor::getDepartureTimeForTrip(const int &trip_id, const int &stop_id) {
  int departure_time = 0;

  return departure_time;
}

int Raptor::getNextTrip(const int &route_id, const int &stop_id) {
  int next_trip = 0;

  return next_trip;
}

