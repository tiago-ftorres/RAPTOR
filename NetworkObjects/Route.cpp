//
// Created by maria on 11/20/2024.
//
#include "Route.h"

void Route::setField(const std::string &field, const std::string &value) {
  fields[field] = value;
}

void Route::addTrip(Trip *trip) {
  trips.push_back(trip);
}

void Route::addStop(Stop *stop) {
  stops.push_back(stop);
}

void Route::sortTrips() {
  std::sort(trips.begin(), trips.end(), [&](const Trip* a, const Trip* b) {
    return Utils::timeToSeconds(a->getStopTimes().front()->arrival_time) <
           Utils::timeToSeconds(b->getStopTimes().front()->arrival_time);
  });
}

const std::string& Route::getField(const std::string &field) const {
  auto it = fields.find(field);
  if (it != fields.end()) {
    return it->second;
  } else {
    throw std::runtime_error("Field not found: " + field);
  }
}

const std::vector<Trip *>& Route::getTrips() const {
  return trips;
}

const std::vector<Stop *>& Route::getStops() const {
  return stops;
}


