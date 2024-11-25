//
// Created by maria on 10/28/2024.
//

#include "Utils.h"

double Utils::manhattan(const double &lat1, const double &lon1, const double &lat2, const double &lon2) {
  return std::abs(lat1 - lat2) + std::abs(lon1 - lon2);
}

int Utils::getDuration(const std::string &string_lat1, const std::string &string_lon1,
                       const std::string &string_lat2, const std::string &string_lon2) {

  double lat1, lon1, lat2, lon2;

  try {

    lat1 = std::stod(string_lat1);
    lon1 = std::stod(string_lon1);
    lat2 = std::stod(string_lat2);
    lon2 = std::stod(string_lon2);

  } catch (const std::invalid_argument &e) {
    throw std::runtime_error("Invalid latitude or longitude format.");
  }

  double average_speed = 5.0; // km/h
  double scaling_factor = 111.0; // Approximately 111 km per degree
  double distance = Utils::manhattan(lat1, lon1, lat2, lon2) * scaling_factor;
  return static_cast<int>(std::round((distance / average_speed) * 60 * 60)); // Seconds
}

std::string Utils::secondsToTime(int seconds) {

  if (seconds == INF) return "INF";

  int hours = seconds / 3600 % 24;
  int minutes = (seconds % 3600) / 60;
  int secs = seconds % 60;

  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << hours << ":"
      << std::setw(2) << std::setfill('0') << minutes << ":"
      << std::setw(2) << std::setfill('0') << secs;
  return oss.str();
}

int Utils::timeToSeconds(const std::string &timeStr) {
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

std::string Utils::trim(const std::string &str) {
  size_t first = str.find_first_not_of(" \t");
  size_t last = str.find_last_not_of(" \t");
  return (first == std::string::npos) ? "" : str.substr(first, (last - first + 1));
}


bool Utils::isNumber(const std::string &str) {
  return std::ranges::all_of(str, [](char c) { return std::isdigit(c); });
}


