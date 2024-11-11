#include <iostream>
#include <iomanip>
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
  std::cout << "Stops number: " << stops.size() << std::endl;
  std::cout << "Trips number: " << trips.size() << std::endl;
  std::cout << "Routes number: " << routes.size() << std::endl;
  std::cout << "Stop Times number: " << stop_times.size() << std::endl;

  Raptor raptor(stops, routes, trips, stop_times);
  Query query = {2, 3, "08:00:00"};

  std::vector<std::vector<JourneyStep>> journeys = raptor.findRoute(query);

  if (journeys.empty()) {
    std::cout << "No journey found :/" << std::endl;
  } else {

    std::cout << "Found " << journeys.size() << " journeys! =) " << std::endl;

    for (const auto &journey: journeys) {
      std::cout << std::endl << "Journey:" << std::endl;
      std::cout << std::setw(5) << "stop" << std::setw(10) << " dep_time " << std::setw(10) << " -> stop " << std::setw(10) << " arr_time " << std::setw(10) << " trip " << std::endl;
      for (const auto &step: journey) {
        std::cout << std::setw(5) << step.stop_src_id << std::setw(10) << secondsToTime(step.departure_time)
                  << std::setw(10) << step.stop_dest_id << std::setw(10) << secondsToTime(step.arrival_time);

        if (step.trip_id != -1)
          std::cout << std::setw(10) << step.trip_id;
        else
          std::cout << std::setw(10) << "footpath";

        std::cout << std::endl;
      }
    }
  }

  return 0;
}
