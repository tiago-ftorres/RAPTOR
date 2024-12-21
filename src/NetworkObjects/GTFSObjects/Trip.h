/**
 * @file Trip.h
 * @brief Defines the Trip class, representing a trip in the GTFS dataset.
 *
 * This header file declares the Trip class, which inherits from GTFSObject.
 * The class serves as a representation of the GTFS "trip.txt" file, storing
 * information about a trip.
 *
 * @author Maria
 * @date 11/20/2024
 */

#ifndef RAPTOR_TRIP_H
#define RAPTOR_TRIP_H

#include "GTFSObject.h"

/**
 * @class Trip
 * @brief Represents a trip in the GTFS data.
 *
 * This class inherits from GTFSObject and manages stop time information
 * for a specific trip. It provides methods for adding stop time keys,
 * retrieving sorted data, and defining custom sorting mechanisms.
 *
 */
class Trip : public GTFSObject {
public:
  /**
   * @brief Adds a stop-time key (trip_id, stop_id) to the trip.
   * @param stop_time_key A pair representing the stop-time key.
   */
  void addStopTimeKey(const std::pair<std::string, std::string> &stop_time_key);

  /**
   * @brief Retrieves the list of stop-time keys.
   * @return A constant reference to the vector of stop-time keys.
   */
  const std::vector<std::pair<std::string, std::string>> &getStopTimesKeys() const;

  /**
   * @brief Sorts the stop times using a custom comparator.
   * @param comparator A function defining the sorting criteria.
   */
  void sortStopTimes(const std::function<bool(const std::pair<std::string, std::string> &,
                                              const std::pair<std::string, std::string> &)> &comparator);

  /**
   * @brief Sets the active status for a specific day.
   * @param day
   * @param is_active
   */
  void setActive(Day day, bool is_active);

  /**
   * @brief Checks if a specific day is active.
   * @param day
   * @return True if the day is active, false otherwise.
   */
  bool isActive(Day day) const;

private:
  std::vector<std::pair<std::string, std::string>> stop_times_keys; ///< Vector of stop-time keys, sorted by stopTime's sequence
  std::unordered_map<Day, bool> active_days_; ///< Map of active days for the trip
};


#endif //RAPTOR_TRIP_H
