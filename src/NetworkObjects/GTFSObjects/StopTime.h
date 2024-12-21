/**
 * @file StopTime.h
 * @brief Defines the StopTime class, representing a stop time in the GTFS dataset.
 *
 * This header file declares the StopTime class, which inherits from GTFSObject.
 * The class serves as a representation of the GTFS "stop_times.txt" file, storing
 * information about a stop time.
 *
 * @author Maria
 * @date 11/20/2024
 */


#ifndef RAPTOR_STOPTIME_H
#define RAPTOR_STOPTIME_H

#include "GTFSObject.h"

/**
 * @class StopTime
 * @brief Represents a stop time in the GTFS data.
 *
 * This class inherits from GTFSObject and manages arrival and departure times
 * for a specific stop. It provides methods for setting and getting arrival and
 * departure times.
 *
 */
class StopTime : public GTFSObject {
public:

  /**
   * @brief Sets the arrival time in seconds.
   * @param seconds The arrival time in seconds.
   */
  void setArrivalSeconds(int seconds);

  /**
   * @brief Sets the departure time in seconds.
   * @param seconds The departure time in seconds.
   */
  void setDepartureSeconds(int seconds);

  /**
   * @brief Retrieves the arrival time in seconds.
   * @return The arrival time in seconds.
   */
  int getArrivalSeconds() const;

  /**
   * @brief Retrieves the departure time in seconds.
   * @return The departure time in seconds.
   */
  int getDepartureSeconds() const;

private:
  int arrival_seconds{}; ///< Arrival time in seconds from midnight.
  int departure_seconds{}; ///< Departure time in seconds from midnight.
};


#endif //RAPTOR_STOPTIME_H
