/**
 * @file StopTime.cpp
 * @brief StopTime class implementation
 *
 * This file contains the implementation of the StopTime class, which represents
 * a stop time in the GTFS dataset.
 *
 * @autor Maria
 * @date 11/20/2024
 */

#include "StopTime.h"

void StopTime::setArrivalSeconds(int seconds){
  arrival_seconds = seconds;
}

void StopTime::setDepartureSeconds(int seconds){
  departure_seconds = seconds;
}

int StopTime::getArrivalSeconds() const {
  return arrival_seconds;
}

int StopTime::getDepartureSeconds() const {
  return departure_seconds;
}
