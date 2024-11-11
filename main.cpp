#include <iostream>
#include "Raptor.h"
#include "Parse.h"
#include "Application.h"

int main(int argc, char *argv[]) {

  std::string inputDirectory;

  if (argc >= 2) {
    inputDirectory = argv[1];
  } else {
    std::cout << "GTFS Input Directory: ";
    std::getline(std::cin, inputDirectory);
  }
  std::unordered_map<int, Agency> agencies = readAgencies(inputDirectory + "/agency.txt");
  std::cout << "Agencies number: " << agencies.size() << std::endl;

  std::unordered_map<int, Calendar> calendars = readCalendars(inputDirectory + "/calendar.txt");
  std::cout << "Calendars number: " << calendars.size() << std::endl;

  std::unordered_map<int, Route> routes = readRoutes(inputDirectory + "/routes.txt");
  std::cout << "Routes number: " << routes.size() << std::endl;

  std::unordered_map<int, Stop> stops = readStops(inputDirectory + "/stops.txt");
  std::cout << "Stops number: " << stops.size() << std::endl;

  std::unordered_map<int, Trip> trips = readTrips(inputDirectory + "/trips.txt");
  std::cout << "Trips number: " << trips.size() << std::endl;

  std::unordered_map<std::pair<int, int>, StopTime, pair_hash> stop_times = readStopTimes(inputDirectory + "/stop_times.txt");
  std::cout << "Stop Times number: " << stop_times.size() << std::endl;

  Raptor raptor(stops, routes, trips, stop_times);

  Application application(raptor);
  application.run();

  return 0;
}
