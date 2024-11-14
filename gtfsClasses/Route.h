//
// Created by maria on 11/14/2024.
//

#ifndef RAPTOR_ROUTE_H
#define RAPTOR_ROUTE_H

#include <string>
#include "DataStructures.h"

// TODO: associate a direction to each route
class Route {
public:

  std::string route_id;
  std::string agency_id;
  std::string route_short_name;
  std::string route_long_name;
  std::string route_desc;
  int route_type;

  std::vector<Trip*> trips; // Trips that follow this route, ordered by earliest to latest arrival time
  std::vector<Stop*> stops; // Stops that compose this route, ordered by stop_sequence

  // Method to get stops starting from a given stop_id
//  auto getStopsAfter(int sequence) const {
//    return std::find_if(stops.begin(), stops.end(), [sequence](Stop* stop) {
//      return stop->stop_sequence > sequence;
//    });
//  }
};

#endif //RAPTOR_ROUTE_H
