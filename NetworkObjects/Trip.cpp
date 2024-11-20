//
// Created by maria on 11/20/2024.
//

#include "Trip.h"

void Trip::setField(const std::string &field, const std::string &value) {
  fields[field] = value;
}

void Trip::addStopTime(StopTime *stopTime) {
  stop_times.push_back(stopTime);
}

const std::string &Trip::getField(const std::string &field) const {
  auto it = fields.find(field);
  if (it != fields.end()) {
    return it->second;
  } else {
    throw std::runtime_error("Field not found: " + field);
  }
}

const std::vector<StopTime *> &Trip::getStopTimes() const {
  return stop_times;
}

void Trip::sortStopTimes() {
  std::sort(stop_times.begin(), stop_times.end(), [](const StopTime* a, const StopTime* b) {
      return a->stop_sequence < b->stop_sequence;
    });
}
