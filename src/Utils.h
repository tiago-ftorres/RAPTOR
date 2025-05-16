/**
 * @file Utils.h
 * @brief Provides utility functions for the RAPTOR application.
 *
 * This header file declares utility functions for the RAPTOR application,
 * including functions for calculating distances, durations, and time conversions.
 *
 * @author Maria
 * @date 10/28/2024
 */

#ifndef RAPTOR_UTILS_H
#define RAPTOR_UTILS_H

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <utility> // for std::pair
#include <vector>
#include <cmath>
#include <hiredis/hiredis.h>

#include "DateTime.h"

/**
 * @class Utils
 * @brief A utility class providing various helper functions.
 *
 * This class contains static utility methods to handle mathematical calculations, time conversions,
 * string manipulations, and date operations. These methods are used throughout the RAPTOR project
 * to simplify code and provide common functionality.
 */
class Utils {
public:
  /**
   * @brief The average speed by foot used for calculations.
   *
   * This constant represents the average speed by foot, which is used
   * for calculating durations based on distances. The value is in kilometers per hour (km/h).
   */
  static constexpr float average_speed = 5.0;

  /**
   * @brief Computes the Manhattan distance between two geographical points.
   *
   * This method calculates the Manhattan (or "taxicab") distance between two points given their
   * latitude and longitude in decimal degrees. This distance is useful for certain types of grid-based
   * calculations.
   *
   * @param[in] lat1 Latitude of the first point.
   * @param[in] lon1 Longitude of the first point.
   * @param[in] lat2 Latitude of the second point.
   * @param[in] lon2 Longitude of the second point.
   * @return The Manhattan distance between the two points.
   */
  static double manhattan(const double &lat1, const double &lon1, const double &lat2, const double &lon2);

  /**
   * @brief Calculates the duration between two geographical points in seconds.
   *
   * This method computes the duration based on the geographic distance between two sets of
   * latitude and longitude coordinates, expressed as strings.
   *
   * @param[in] string_lat1 Latitude of the first point as a string.
   * @param[in] string_lon1 Longitude of the first point as a string.
   * @param[in] string_lat2 Latitude of the second point as a string.
   * @param[in] string_lon2 Longitude of the second point as a string.
   * @return The duration in seconds.
   */
  static int getDuration(const std::string &string_lat1, const std::string &string_lon1,
                         const std::string &string_lat2, const std::string &string_lon2);

  /**
   * @brief Calculates the duration of the footpath given the distance.
   *
   * @param distance Distance of the footpath in kilometers.
   * @return The duration in seconds.
   */
  static int getDuration(double distance);

  /**
   * @brief Converts a time in seconds to a string format (HH:MM:SS).
   *
   * This method converts a given time in seconds into a formatted string representing the time
   * in the "HH:MM:SS" format.
   *
   * @param[in] seconds The time in seconds.
   * @return A string representation of the time in "HH:MM:SS" format.
   */
  static std::string secondsToTime(std::optional<int> seconds);

  /**
   * @brief Converts a time string to the equivalent number of seconds.
   *
   * This method converts a time string (e.g., "12:30:00") to the total number of seconds.
   *
   * @param[in] timeStr A time string in the "HH:MM:SS" format.
   * @return The total time in seconds.
   */
  static int timeToSeconds(const std::string &timeStr);

  /**
   * @brief Converts a Time object to the equivalent number of seconds.
   *
   * This method converts a Time object to the total number of seconds since midnight.
   *
   * @param[in] time A Time object representing a specific time.
   * @return The total time in seconds.
   */
  static int timeToSeconds(const Time &time);

  /**
   * @brief Splits a string into a vector of substrings based on a delimiter.
   *
   * This method splits a string into parts wherever a specified delimiter appears.
   *
   * @param[in] str The input string to be split.
   * @param[in] delimiter The delimiter character to split the string by.
   * @return A vector of substrings split from the input string.
   */
  static std::vector<std::string> split(const std::string &str, char delimiter);

  /**
   * @brief Retrieves the first word in a string.
   *
   * This method extracts and returns the first word from a given string, stopping at the first space.
   *
   * @param[in] str The input string.
   * @return The first word in the string.
   */
  static std::string getFirstWord(const std::string &str);

  /**
   * @brief Trims leading and trailing whitespace from a string.
   *
   * This method removes any leading or trailing whitespace from the given string.
   *
   * @param[in,out] line The line to be cleaned.
   */
  static void clean(std::string &input);

  /**
   * @brief Checks if a string represents a valid number.
   *
   * This method checks whether the input string can be interpreted as a valid numerical value.
   *
   * @param[in] str The input string to be checked.
   * @return True if the string is a valid number, false otherwise.
   */
  static bool isNumber(const std::string &str);

  /**
   * @brief Retrieves the number of days in a specific month of a specific year.
   *
   * This method returns the number of days in a given month, accounting for leap years if applicable.
   *
   * @param[in] year The year of interest.
   * @param[in] month The month of interest (1-12).
   * @return The number of days in the specified month of the specified year.
   */
  static int daysInMonth(int year, int month);

  /**
   * @brief Checks if a date is within a specified date range.
   *
   * This method checks whether a given date falls within the specified range of start and end dates.
   *
   * @param[in] date The date to be checked.
   * @param[in] start_date The start of the date range (in string format).
   * @param[in] end_date The end of the date range (in string format).
   * @return True if the date is within the range, false otherwise.
   */
  static bool dateWithinRange(const Date &date, const std::string &start_date, const std::string &end_date);

  /**
   * @brief Adds one day to a given date.
   *
   * This method increments the given date by one day.
   *
   * @param[in] date The date to which one day should be added.
   * @return The resulting date after adding one day.
   */
  static Date addOneDay(Date date);

  /**
   * @brief Converts a Day enum to a string representation.
   *
   * This method converts a Day enum (Current or Next) to its string representation.
   *
   * @param[in] day The Day enum to be converted.
   * @return The string representation of the specified day.
   */
  static std::string dayToString(Day day);

  /**
   * @brief Establishes a connection to the Redis server.
   *
   * This function attempts to connect to a Redis server running on the local machine
   * (127.0.0.1) at port 6379. If the connection is successful, it returns a pointer to
   * the `redisContext`. If the connection fails, it ouputs an error message and
   * returns a `nullptr`.
   *
   * @return A pointer to the `redisContext` if the connection is successful, otherwise `nullptr`.
   */
  static redisContext* connectToRedis();

  /**
   * @brief Retrieves the footpath distance between two stops from Redis
   *
   * This function looks up the footpath distance between two stops (identified by stop IDs)
   * from a Redis database. The distance is stored with keys in the format "stop1:stop2" and
   * are bidirectional.
   *
   * The values in Redis are expected to be string representations of numeric distances (in kilometers),
   * which this function converts to double
   *
   * @param c Pointer to an active Redis context
   * @param stop1 the ID of the first stop
   * @param stop2 the ID of the second stop
   * @return The footpath distance between the two stops as a double if found, or -1 not found.
   */
  static double getDistance(redisContext *c, const std::string &stop1, const std::string &stop2);
};

#endif //RAPTOR_UTILS_H
