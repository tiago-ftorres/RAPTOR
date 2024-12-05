//
// Created by maria on 12/3/2024.
//

#ifndef RAPTOR_DATETIME_H
#define RAPTOR_DATETIME_H

#include <optional>

constexpr const char* weekdays_names[] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};

struct Date {
  int year;
  int month;
  int day;
  int weekday; // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
};

enum class Day {
  CurrentDay,
  NextDay
};

struct Time {
  int hours;
  int minutes;
  int seconds;
};

#endif //RAPTOR_DATETIME_H
