//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_STOPTIME_H
#define RAPTOR_STOPTIME_H

#include "GTFSObject.h"

class StopTime : public GTFSObject {
public:

  void setArrivalSeconds(int seconds);

  void setDepartureSeconds(int seconds);

  int getArrivalSeconds() const;

  int getDepartureSeconds() const;

private:
  int arrival_seconds{};
  int departure_seconds{};
};


#endif //RAPTOR_STOPTIME_H
