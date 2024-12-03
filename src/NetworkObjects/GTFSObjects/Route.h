//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_ROUTE_H
#define RAPTOR_ROUTE_H

#include "GTFSObject.h"

class Route : public GTFSObject {
public:
  void addTripId(const std::string& trip_id);

  void addStopId(const std::string& stop_id);

  const std::vector<std::string>& getTripsIds() const;

  const std::vector<std::string>& getStopsIds() const;

  void sortTrips(const std::function<bool(const std::string &, const std::string &)> &comparator);

private:
  std::vector<std::string> trips_ids; // Sorted by earliest to latest arrival time
  std::vector<std::string> stops_ids; // Sorted by stop_sequence
};

#endif //RAPTOR_ROUTE_H
