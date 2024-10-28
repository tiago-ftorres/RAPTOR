#include <iostream>
#include "Raptor.h"
#include "Parse.h"

int main() {
  // TODO: input directory
  std::cout << "Hello, World!" << std::endl;
  auto agencies = readAgencies("./dataset/Porto/gtfs/agency.csv");
  auto calendars = readCalendars("./dataset/Porto/gtfs/calendar.csv");
  auto routes = readRoutes("./dataset/Porto/gtfs/routes.csv");
  auto stops = readStops("./dataset/Porto/gtfs/stops.csv");
  auto trips = readTrips("./dataset/Porto/gtfs/trips.csv");
  auto stop_times = readStopTimes("./dataset/Porto/gtfs/stop_times.csv");

  std::cout << "Agencies number: " << agencies.size() << std::endl;
  std::cout << "Calendars number: " << calendars.size() << std::endl;
  std::cout << "Routes number: " << routes.size() << std::endl;
  std::cout << "Stops number: " << stops.size() << std::endl;
  std::cout << "Trips number: " << trips.size() << std::endl;
  std::cout << "Stop Times number: " << stop_times.size() << std::endl;

  Raptor raptor(stops, routes, trips, stop_times);

  Query query = {2, 3, "08:00:00"};
  int min_arrival_time = raptor.findRoute(query);
  if (min_arrival_time == INF) {
    std::cout << "No route found :/" << std::endl;
  } else {
    std::cout << "Earliest arrival time: " << secondsToTime(min_arrival_time) << std::endl;
  }

  return 0;
}
