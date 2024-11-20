//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_STOP_H
#define RAPTOR_STOP_H

#include <stdexcept>
#include "DataStructures.h"
#include "Utils.h"

class Stop {
  public:
    void setField(const std::string& field, const std::string& value);
    void addStopTime(StopTime* stopTime);
    void addRouteKey(const std::pair<std::string, std::string>& route_key);
    void addFootpath(const std::string& other_id, int duration);

    const std::string& getField(const std::string& field) const;
    const std::vector<StopTime*>& getStopTimes() const;
    const std::vector<std::pair<std::string, std::string>>& getRouteKeys() const;
    const std::unordered_map<std::string, Footpath>& getFootpaths() const;

    void sortStopTimes();

  private:
    std::unordered_map<std::string, std::string> fields; // Based on .txt header

    std::vector<StopTime*> stop_times; // Sorted by earliest to latest departure time
    std::vector<std::pair<std::string, std::string>> routes_keys; // (route_id, direction_id)
    std::unordered_map<std::string, Footpath> footpaths;

};


#endif //RAPTOR_STOP_H
