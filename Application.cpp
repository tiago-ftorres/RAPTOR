//
// Created by maria on 11/11/2024.
//
#include "Application.h"

Application::Application(std::string  inputDirectory)
        : inputDirectory(std::move(inputDirectory)){}

void Application::run() {
  Parser parser(inputDirectory);

  std::unordered_map<std::string, Trip> trips = parser.getTrips();
  std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> routes = parser.getRoutes();
  std::unordered_map<std::string, Stop> stops = parser.getStops();
  std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> stop_times = parser.getStopTimes();

  Raptor raptor(stops, routes, trips, stop_times);

  std::string command;
  showCommands();

  while (true) {
    std::cout << "\nType a command: ";
    std::getline(std::cin, command);

    if (command.rfind("query", 0) == 0) {
      handleQuery(raptor, command);
    } else if (command == "help") {
      showCommands();
    } else if (command == "quit") {
      std::cout << "Quitting program..." << std::endl;
      break;
    } else {
      std::cout << "Invalid command. :/" << std::endl;
      showCommands();
    }

  }
}

void Application::handleQuery(Raptor& raptor,const std::string &command) {

  // TODO: ask for input directory here
  // TODO: create raptor instance here
  // TODO: ask for query here
  std::string source, target, departure_time;

  std::istringstream iss(command);
  std::string queryCommand;

  iss >> queryCommand >> source >> target >> departure_time;

  Query query = {source, target, departure_time};
  std::vector<std::vector<JourneyStep>> journeys = raptor.findJourneys(query);

  if (journeys.empty()) {
    std::cout << "No journey found :/" << std::endl;
  } else {

    std::cout << "Found " << journeys.size() << " journeys! =) " << std::endl;

    for (const auto &journey: journeys) {
      std::cout << std::endl << "Journey:" << std::endl;
      std::cout << std::setw(6) << " trip " << std::setw(5) << "stop" << std::setw(10) << " dep_time " << std::setw(10) << " -> stop " << std::setw(10) << " arr_time " << std::endl;

      for (const auto &step: journey) {
        if (step.trip_id != "-1")
          std::cout << std::setw(6) << step.trip_id;
        else
          std::cout << std::setw(6) << "footpath";

        std::cout << std::setw(5) << step.stop_src_id << std::setw(10) << Utils::secondsToTime(step.departure_time)
                  << std::setw(10) << step.stop_dest_id << std::setw(10) << Utils::secondsToTime(step.arrival_time);

        std::cout << std::endl;
      }
    }
  }

}

void Application::showCommands() {
  std::cout << std::endl << "Available commands:" << std::endl;


  std::cout << std::left << std::setw(45) << " 1. query <source_id> <target_id> <departure_time>" <<" Runs RAPTOR from source to target at departure_time." << std::endl;
  std::cout << "     example: query 5777 5776 22:00:00" << std::endl;
  std::cout << std::left << std::setw(45) << " 2. help " << " Shows available commands. "<< std::endl;
  std::cout << " 3. quit " << std::endl;
}
