//
// Created by maria on 10/28/2024.
//

#ifndef RAPTOR_UTILS_H
#define RAPTOR_UTILS_H

#include <utility> // for std::pair
#include <cmath>   // for std::sin, std::cos, std::atan2, std::sqrt
#include <string>
#include "DataStructures.h"

double haversine(const Coordinates& coord1, const Coordinates& coord2);
int getDuration(const Coordinates& coord1, const Coordinates& coord2);

std::string secondsToTime(int seconds);
int timeToSeconds(const std::string& timeStr);

#endif //RAPTOR_UTILS_H
