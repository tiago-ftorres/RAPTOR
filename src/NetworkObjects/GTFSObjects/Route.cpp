//
// Created by maria on 11/20/2024.
//
#include "Route.h"
#include <iostream>

void Route::addTripId(const std::string& trip_id) {
  trips_ids.push_back(trip_id);
}

void Route::addStopId(const std::string& stop_id) {
  stops_ids.push_back(stop_id);
}

void Route::sortTrips(const std::function<bool(const std::string &, const std::string &)> &comparator) {
  std::sort(trips_ids.begin(), trips_ids.end(), comparator);
}

const std::vector<std::string> &Route::getTripsIds() const {
  return trips_ids;
}

const std::vector<std::string> &Route::getStopsIds() const {
  return stops_ids;
}


