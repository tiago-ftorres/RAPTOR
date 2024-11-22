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
double Utils::haversine(const double& lat1, const double& lon1,
                        const double& lat2, const double& lon2) {

  double lat1_radians = lat1 * M_PI / 180.0; // Degrees to radians
  double lon1_radians = lon1 * M_PI / 180.0;
  double lat2_radians = lat2 * M_PI / 180.0;
  double lon2_radians = lon2 * M_PI / 180.0;

  double diff_lat = lat2_radians - lat1_radians;
  double diff_lon = lon2_radians - lon1_radians;

  double a = std::sin(diff_lat / 2) * std::sin(diff_lat / 2) +
             std::cos(lat1_radians) * std::cos(lat2_radians) *
             std::sin(diff_lon / 2) * std::sin(diff_lon / 2);
  double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

  return EARTH_RADIUS * c; // Kilometers
}

int Utils::getDuration(const std::string& string_lat1, const std::string& string_lon1,
                        const std::string& string_lat2, const std::string& string_lon2) {

  double lat1, lon1, lat2, lon2;

  try {

    lat1 = std::stod(string_lat1);
    lon1 = std::stod(string_lon1);
    lat2 = std::stod(string_lat2);
    lon2 = std::stod(string_lon2);

  } catch (const std::invalid_argument& e) {
    throw std::runtime_error("Invalid latitude or longitude format.");
  }

  double average_speed = 5.0; // km/h
  return (int) ((Utils::haversine(lat1, lon1, lat2, lon2)/average_speed) * 60 * 60); // Seconds
}

std::string Utils::secondsToTime(int seconds) {

  if (seconds == INF)
    return "INF";

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

std::vector<std::string> Utils::split(const std::string &str, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(str);

  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }

  if (str.back() == delimiter) {
    tokens.emplace_back("");  // Add empty token if the last character is the delimiter
  }

  return tokens;
}


std::string Utils::getFirstWord(const std::string &str) {
  return str.substr(0, str.find(' '));
}

