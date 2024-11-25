//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_STOP_H
#define RAPTOR_STOP_H

#include "GTFSObject.h"
#include "StopTime.h"
#include "../DataStructures.h"

class Stop : public GTFSObject {
  public:
    void addStopTime(StopTime* stopTime);
    void addRouteKey(const std::pair<std::string, std::string>& route_key);
    void addFootpath(const std::string& other_id, int& duration);

    const std::vector<StopTime*>& getStopTimes() const;
    const std::vector<std::pair<std::string, std::string>>& getRouteKeys() const;
    const std::unordered_map<std::string, Footpath>& getFootpaths() const;

    void sortStopTimes();

  private:
    std::vector<StopTime*> stop_times; // Sorted by earliest to latest departure time
    std::vector<std::pair<std::string, std::string>> routes_keys; // (route_id, direction_id)
    std::unordered_map<std::string, Footpath> footpaths;

};


#endif //RAPTOR_STOP_H
