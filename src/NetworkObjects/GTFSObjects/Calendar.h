/**
 * @file Calendar.h
 * @brief Defines the Calendar class, representing active weekdays for calendar in the GTFS dataset.
 *
 * This header file declares the Calendar class, which inherits from GTFSObject.
 * The class serves as a representation of the GTFS "calendar.txt" file, storing
 * information about active days of a calendar.
 *
 * @author Maria
 * @date 11/20/2024
 */

#ifndef RAPTOR_CALENDAR_H
#define RAPTOR_CALENDAR_H

#include "GTFSObject.h"

/**
 * @class Calendar
 * @brief Represents active weekdays for calendar in the GTFS data.
 *
 * This class inherits from GTFSObject and encapsulates the details of active weekdays for calendar.
 *
 * * @note This class currently acts as a placeholder and can be extended
 * with specific attributes and methods relevant to active weekdays for calendar.
 */
class Calendar : public GTFSObject  {

};

#endif //RAPTOR_CALENDAR_H
