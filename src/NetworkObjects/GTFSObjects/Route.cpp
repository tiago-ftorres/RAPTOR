//
// Created by maria on 11/20/2024.
//
#include "Route.h"

void Route::addTrip(Trip *trip) {
  trips.push_back(trip);
}

void Route::addStop(Stop *stop) {
  stops.push_back(stop);
}

void Route::sortTrips() {
  std::sort(trips.begin(), trips.end(), [&](const Trip* a, const Trip* b) {
    return Utils::timeToSeconds(a->getStopTimes().front()->getField("arrival_time")) <
           Utils::timeToSeconds(b->getStopTimes().front()->getField("arrival_time"));
  });
}

const std::vector<Trip *>& Route::getTrips() const {
  return trips;
}

const std::vector<Stop *>& Route::getStops() const {
  return stops;
}


