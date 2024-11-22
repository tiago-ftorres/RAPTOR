//
// Created by maria on 11/11/2024.
//
#include "Application.h"

Application::Application(std::string  inputDirectory)
        : inputDirectory(std::move(inputDirectory)){}

void Application::run() {
  Parser parser(inputDirectory);

  std::unordered_map<std::string, Agency> agencies = parser.getAgencies();
  std::unordered_map<std::string, Calendar> calendars = parser.getCalendars();
  std::unordered_map<std::string, Trip> trips = parser.getTrips();
  std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> routes = parser.getRoutes();
  std::unordered_map<std::string, Stop> stops = parser.getStops();
  std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> stop_times = parser.getStopTimes();

  Raptor raptor(agencies, calendars, stops, routes, trips, stop_times);

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

  std::string source, target, departure_time;

  std::istringstream iss(command);
  std::string queryCommand;

  iss >> queryCommand >> source >> target >> departure_time;

  Query query = {source, target, departure_time};
  std::vector<std::vector<JourneyStep>> journeys = raptor.findJourneys(query);

  if (journeys.empty()) {
    std::cout << "No journey found :/" << std::endl;
  } else {

    std::cout << "Found " << journeys.size() << " journey(s)! =) " << std::endl;

    for (int i = 0 ; i < journeys.size(); i++){
      const std::vector<JourneyStep>& journey = journeys[i];
      std::cout << std::endl << "Journey " << i + 1 << ": " << std::endl << std::endl;
      std::cout << std::setw(5) << "step"<< std::setw(13) << " trip " << std::setw(22) << "stop" << std::setw(10) << "dep_time "
                << std::setw(22) << "-> stop " << std::setw(9) << "arr_time " << std::endl;

      for (int j = 0 ; j < journey.size() ; j++){
        const JourneyStep& step = journey[j];
        std::cout << std::setw(6) << j + 1;

        if (step.trip_id != "-1")
          std::cout << std::setw(12) << step.trip_id;
        else
          std::cout << std::setw(12) << "footpath";

        std::cout << std::setw(22) << step.src_stop->getField("stop_name") << std::setw(10) << Utils::secondsToTime(step.departure_time)
                  << std::setw(22) << step.dest_stop->getField("stop_name") << std::setw(9) << Utils::secondsToTime(step.arrival_time);

        std::cout << std::endl;
      }
    }
  }

}

void Application::showCommands() {
  std::cout << std::endl << "Available commands:" << std::endl;


  std::cout << std::left << std::setw(55) << " 1. query <source_id> <target_id> <departure_time>" <<" Runs RAPTOR from source to target at departure_time." << std::endl;
  std::cout << "     example: query 5777 5776 22:00:00" << std::endl;
  std::cout << "              query 5775 5813 03:00:33" << std::endl;
  std::cout << "              query 5746 5756 12:22:33" << std::endl;
  std::cout << "              query 5753 5782 19:44:00" << std::endl;
  std::cout << "              query 5726 5739 06:44:00" << std::endl; // Metro Trindade to Lidador
  std::cout << "              query SAL2 IPO5 14:00:00" << std::endl; // STCP Salgueiros to IPO
  std::cout << std::left << std::setw(55) << " 2. help " << " Shows available commands. "<< std::endl;
  std::cout << " 3. quit " << std::endl;
}
