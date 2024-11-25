//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_ROUTE_H
#define RAPTOR_ROUTE_H

#include "GTFSObject.h"
#include "Trip.h"
#include "Stop.h"

class Route : public GTFSObject {
public:
  void addTrip(Trip* trip);

  void addStop(Stop* stop);

  const std::vector<Trip*>& getTrips() const;

  const std::vector<Stop*>& getStops() const;

  void sortTrips();

private:
  std::vector<Trip*> trips; // Sorted by earliest to latest arrival time
  std::vector<Stop*> stops; // Sorted by stop_sequence
};

#endif //RAPTOR_ROUTE_H
