//
// Created by maria on 11/20/2024.
//

#include "Trip.h"

void Trip::addStopTimeKey(const std::pair<std::string, std::string> &stop_time_key) {
  stop_times_keys.push_back(stop_time_key);
}

const std::vector<std::pair<std::string, std::string>> &Trip::getStopTimesKeys() const {
  return stop_times_keys;
}

void Trip::sortStopTimes(const std::function<bool(const std::pair<std::string, std::string>&,
                                                  const std::pair<std::string, std::string>&)>& comparator) {
  std::sort(stop_times_keys.begin(), stop_times_keys.end(), comparator);
}

bool Trip::isActive(Day day) const {
  return active_days_.at(day);
}

void Trip::setActive(Day day, bool is_active) {
  active_days_[day] = is_active;
}
