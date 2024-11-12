//
// Created by maria on 10/28/2024.
//

#ifndef RAPTOR_UTILS_H
#define RAPTOR_UTILS_H

#include <utility> // for std::pair
#include <cmath>   // for std::sin, std::cos, std::atan2, std::sqrt
#include "DataStructures.h"

class Utils {
public:
  static double haversine(const Coordinates& coord1, const Coordinates& coord2);
  static int getDuration(const Coordinates& coord1, const Coordinates& coord2);

  static std::string secondsToTime(int seconds);
  static int timeToSeconds(const std::string& timeStr);
};

#endif //RAPTOR_UTILS_H
