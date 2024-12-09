//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_TRIP_H
#define RAPTOR_TRIP_H

#include "GTFSObject.h"

class Trip : public GTFSObject {
public:
  void addStopTimeKey(const std::pair<std::string, std::string> &stop_time_key);

  const std::vector<std::pair<std::string, std::string>> &getStopTimesKeys() const;

  void sortStopTimes(const std::function<bool(const std::pair<std::string, std::string> &,
                                              const std::pair<std::string, std::string> &)> &comparator);

  void setActive(Day day, bool is_active);

  bool isActive(Day day) const;

private:
  std::vector<std::pair<std::string, std::string>> stop_times_keys; // Sorted by stop_times' sequence
  std::unordered_map<Day, bool> active_days_; // Active days for this trip
};


#endif //RAPTOR_TRIP_H
