//
// Created by maria on 12/3/2024.
//

#ifndef RAPTOR_DATETIME_H
#define RAPTOR_DATETIME_H

#include <optional>

struct Date {
  int year;
  int month;
  int day;
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
