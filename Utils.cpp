//
// Created by maria on 10/28/2024.
//

#include <sstream>
#include <iomanip>
#include <iostream>
#include "Utils.h"

// Earth radius in kilometers
const double EARTH_RADIUS = 6371.0;

// Haversine formula
double Utils::haversine(const Coordinates& coord1, const Coordinates& coord2) {
  double lat1 = coord1.lat * M_PI / 180.0; // Degrees to radians
  double lon1 = coord1.lon * M_PI / 180.0;
  double lat2 = coord2.lat * M_PI / 180.0;
  double lon2 = coord2.lon * M_PI / 180.0;

  double dlat = lat2 - lat1;
  double dlon = lon2 - lon1;

  double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
             std::cos(lat1) * std::cos(lat2) *
             std::sin(dlon / 2) * std::sin(dlon / 2);
  double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

  return EARTH_RADIUS * c; // Kilometers
}

int Utils::getDuration(const Coordinates& coord1, const Coordinates& coord2) {
  double average_speed = 5.0; // km/h
  return (Utils::haversine(coord1, coord2)/average_speed) * 60 * 60; // Seconds
}

std::string Utils::secondsToTime(int seconds) {
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int secs = seconds % 60;

  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << hours << ":"
      << std::setw(2) << std::setfill('0') << minutes << ":"
      << std::setw(2) << std::setfill('0') << secs;
  return oss.str();
}

int Utils::timeToSeconds(const std::string& timeStr) {
  int hours, minutes, seconds;
  char colon;

  std::istringstream iss(timeStr);
  iss >> hours >> colon >> minutes >> colon >> seconds;

  return hours * 3600 + minutes * 60 + seconds;
}
