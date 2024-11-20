//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_ROUTE_H
#define RAPTOR_ROUTE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "DataStructures.h"
#include "Utils.h"

class Route {
public:
  void setField(const std::string& field, const std::string& value);

  void addTrip(Trip* trip);
  void addStop(Stop* stop);

  std::string getField(const std::string& field) const;

  const std::vector<Trip*>& getTrips() const;
  const std::vector<Stop*>& getStops() const;

  void sortTrips();

private:

  std::unordered_map<std::string, std::string> fields; // Based on .txt header
  std::vector<Trip*> trips; // Trips that follow this route, ordered by earliest to latest arrival time
  std::vector<Stop*> stops; // Stops that compose this route, ordered by stop_sequence
};

#endif //RAPTOR_ROUTE_H
