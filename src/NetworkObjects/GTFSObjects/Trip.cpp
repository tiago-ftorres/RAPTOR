//
// Created by maria on 11/20/2024.
//

#include "Trip.h"

void Trip::addStopTime(StopTime *stopTime) {
  stop_times.push_back(stopTime);
}

const std::vector<StopTime *> &Trip::getStopTimes() const {
  return stop_times;
}

void Trip::sortStopTimes() {
  std::sort(stop_times.begin(), stop_times.end(), [](const StopTime* a, const StopTime* b) {
      int seq_a = std::stoi(a->getField("stop_sequence"));
      int seq_b = std::stoi(b->getField("stop_sequence"));
      return seq_a < seq_b;
    });
}
