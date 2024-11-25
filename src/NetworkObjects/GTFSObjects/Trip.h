//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_TRIP_H
#define RAPTOR_TRIP_H

#include "GTFSObject.h"
#include "StopTime.h"

class Trip : public GTFSObject {
  public:
    void addStopTime(StopTime* stopTime);

    const std::vector<StopTime*>& getStopTimes() const;

    void sortStopTimes();

  private:
    std::vector<StopTime*> stop_times; // Sorted by stop_times' sequence
};


#endif //RAPTOR_TRIP_H
