//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_TRIP_H
#define RAPTOR_TRIP_H

#include <stdexcept>
#include "DataStructures.h"

class Trip {
  public:
    void setField(const std::string& field, const std::string& value);
    void addStopTime(StopTime* stopTime);

    const std::string& getField(const std::string& field) const;
    const std::vector<StopTime*>& getStopTimes() const;

    void sortStopTimes();

  private:

    std::unordered_map<std::string, std::string> fields; // Based on .txt header
    std::vector<StopTime*> stop_times; // Sorted by stop_times' sequence

};


#endif //RAPTOR_TRIP_H
