#include <iostream>
#include "Raptor.h"
#include "Parse.h"

int main(int argc, char *argv[]) {

  std::string inputDirectory;

  if (argc >= 2) {
    inputDirectory = argv[1];
  } else {
    std::cout << "GTFS Input Directory: ";
    std::getline(std::cin, inputDirectory);
  }

  auto agencies = readAgencies(inputDirectory + "/agency.csv");
  auto calendars = readCalendars(inputDirectory + "/calendar.csv");
  auto routes = readRoutes(inputDirectory + "/routes.csv");
  auto stops = readStops(inputDirectory + "/stops.csv");
  auto trips = readTrips(inputDirectory + "/trips.csv");
  auto stop_times = readStopTimes(inputDirectory + "/stop_times.csv");

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
