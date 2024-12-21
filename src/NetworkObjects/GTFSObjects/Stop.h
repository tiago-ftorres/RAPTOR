/**
 * @file Stop.h
 * @brief Defines the Stop class, representing a stop in the GTFS dataset.
 *
 * This header file declares the Stop class, which inherits from GTFSObject.
 * The class serves as a representation of the GTFS "stop.txt" file, storing
 * information about a stop.
 *
 * @author Maria
 * @date 11/20/2024
 */
//
// Created by maria on 11/20/2024.
//

#ifndef RAPTOR_STOP_H
#define RAPTOR_STOP_H

#include "GTFSObject.h"
#include "../DataStructures.h"

/**
 * @class Stop
 * @brief Represents a stop in the GTFS data.
 *
 * This class inherits from GTFSObject and manages stop time and route information
 * for a specific stop. It provides methods for adding stop time and route IDs,
 * retrieving sorted data, and defining custom sorting mechanisms.
 *
 */
class Stop : public GTFSObject {
public:
  /**
   * @brief Adds a stop-time key (trip_id, stop_id) to the stop.
   * @param stop_time_key A pair representing the stop-time key.
   */
  void addStopTimeKey(const std::pair<std::string, std::string> &stop_time_key);

  /**
   * @brief Adds a route key (route_id, direction_id) to the stop.
   * @param route_key A pair representing the route key.
   */
  void addRouteKey(const std::pair<std::string, std::string> &route_key);

  /**
   * @brief Adds a footpath to another stop.
   * @param other_id The ID of the other stop.
   * @param duration The duration of the footpath in seconds.
   */
  void addFootpath(const std::string &other_id, int &duration);

  /**
   * @brief Retrieves the list of stop-time keys.
   * @return A constant reference to the vector of stop-time keys.
   */
  const std::vector<std::pair<std::string, std::string>> &getStopTimesKeys() const;

  /**
  * @brief Retrieves the set of route keys.
  * @return A constant reference to the unordered set of route keys.
    */
  const std::unordered_set<std::pair<std::string, std::string>, pair_hash> &getRouteKeys() const;

  /**
   * @brief Retrieves the map of footpaths.
   * @return A constant reference to the map of footpaths.
   */
  const std::unordered_map<std::string, int> &getFootpaths() const;

  /**
   * @brief Sorts the stop times using a custom comparator.
   * @param comparator A function defining the sorting criteria.
   */
  void sortStopTimes(const std::function<bool(const std::pair<std::string, std::string> &,
                                              const std::pair<std::string, std::string> &)> &comparator);

private:
  std::vector<std::pair<std::string, std::string>> stop_times_keys; ///< Vector of stop-time keys, sorted by earliest departure time
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> routes_keys; ///< Set of route keys
  std::unordered_map<std::string, int> footpaths; ///< Map of footpaths to other stops

};


#endif //RAPTOR_STOP_H
