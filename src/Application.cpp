//
// Created by maria on 11/11/2024.
//
#include "Application.h"

Application::Application(std::string inputDirectory)
        : inputDirectory(std::move(inputDirectory)) {}

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

    std::string trimmedCommand = Utils::trim(command);

    if (trimmedCommand == "query") {
      handleQuery(raptor);
    } else if (trimmedCommand == "help") {
      showCommands();
    } else if (trimmedCommand == "quit") {
      std::cout << "Quitting program..." << std::endl;
      break;
    } else {
      std::cout << "Invalid command. :/" << std::endl;
      showCommands();
    }
  }
}

//std::cout << "     example: query 5777 5776 22:00:00" << std::endl;
//  std::cout << "              query 5775 5813 03:00:33" << std::endl;
//  std::cout << "              query 5746 5756 12:22:33" << std::endl;
//  std::cout << "              query 5753 5782 19:44:00" << std::endl;
//  std::cout << "              query 5726 5739 06:44:00" << std::endl; // Metro Trindade to Lidador
//  std::cout << "              query SAL2 IPO5 14:00:00" << std::endl; // STCP Salgueiros to IPO
//  std::cout << "              query MAIA3 PARR3 5:55:55" << std::endl; // STCP Maia to Arrabida

void Application::handleQuery(Raptor &raptor) {

  std::string source, target, departure_time;

  while (true) {
    std::cout << "Enter source stop id: ";
    std::getline(std::cin, source);
    source = Utils::trim(source);

    if (raptor.getStops().find(source) != raptor.getStops().end()) break;
    else
      std::cout << "Invalid source stop id. Please try again. Example: 5753 for Metro or SAL2 for STCP." << std::endl;
  }

  while (true) {
    std::cout << "Enter target stop id: ";
    std::getline(std::cin, target);
    target = Utils::trim(target);

    if (raptor.getStops().find(target) != raptor.getStops().end()) break;
    else
      std::cout << "Invalid target stop id. Please try again. Example: 5753 for Metro or SAL2 for STCP." << std::endl;
  }

  std::string input;

  // Ask for hours
  int hours;
  while (true) {
    std::cout << "Enter hours (0-23): ";
    std::getline(std::cin, input);
    input = Utils::trim(input);
    if (Utils::isNumber(input)) {
      hours = std::stoi(input);
      if (hours >= 0 && hours <= 23) break;
    }
    std::cout << "Invalid hours. Please enter a valid hour between 0 and 23.\n";
  }

  // Ask for minutes
  int minutes;
  while (true) {
    std::cout << "Enter minutes (0-59): ";
    std::getline(std::cin, input);
    input = Utils::trim(input);
    if (Utils::isNumber(input)) {
      minutes = std::stoi(input);
      if (minutes >= 0 && minutes <= 59) break;
    }
    std::cout << "Invalid minutes. Please enter valid minutes between 0 and 59.\n";
  }

  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << hours << ":"
      << std::setw(2) << std::setfill('0') << minutes << ":00";
  departure_time = oss.str();

//  departure_time = std::to_string(hours) + ":" + std::to_string(minutes) + ":00";

  Query query = {source, target, departure_time};
  raptor.setQuery(query);

  auto start_time = std::chrono::high_resolution_clock::now();

  std::vector<std::vector<JourneyStep>> journeys = raptor.findJourneys();

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

  std::cout << "Took " << duration << " ms (" << std::round(duration / 1000.0) << " seconds) to look for journeys." << std::endl;

  if (journeys.empty()) std::cout << "No journey found :/" << std::endl;
  else {
    std::cout << "Found " << journeys.size() << " journey(s)! =) " << std::endl;

    for (int i = 0; i < journeys.size(); i++) {
      const std::vector<JourneyStep> &journey = journeys[i];
      int journey_duration = journey.back().arrival_time - journey.front().departure_time;
      std::cout << std::endl << "Journey " << i + 1 << " (" << Utils::secondsToTime(journey_duration) << "): " << std::endl << std::endl;
      Raptor::showJourney(journey);
    }
  }

//  std::cout << "Footpath duration from CCB1 to PAL4: "
//            << raptor.getStops().at("CCB1").getFootpaths().at("PAL4").duration << " seconds." << std::endl;
//  std::cout << "Footpath duration from PAL4 to CVLO: "
//            << raptor.getStops().at("PAL4").getFootpaths().at("CVLO").duration << " seconds." << std::endl;
//  std::cout << "Sum: " << raptor.getStops().at("CCB1").getFootpaths().at("PAL4").duration +
//                          raptor.getStops().at("PAL4").getFootpaths().at("CVLO").duration << " seconds." << std::endl;
//  std::cout << "Footpath duration from CCB1 to CVLO: "
//            << raptor.getStops().at("CCB1").getFootpaths().at("CVLO").duration << " seconds." << std::endl << std::endl;
//
//  std::cout << "Footpath duration from MTT2 to BJ1: " << raptor.getStops().at("MTT2").getFootpaths().at("BJ1").duration
//            << " seconds." << std::endl;
//  std::cout << "Footpath duration from BJ1 to CVLO: " << raptor.getStops().at("BJ1").getFootpaths().at("CVLO").duration
//            << " seconds." << std::endl;
//  std::cout << "Sum: " << raptor.getStops().at("MTT2").getFootpaths().at("BJ1").duration +
//                          raptor.getStops().at("BJ1").getFootpaths().at("CVLO").duration << " seconds." << std::endl;
//  std::cout << "Footpath duration from MTT2 to CVLO: "
//            << raptor.getStops().at("MTT2").getFootpaths().at("CVLO").duration << " seconds." << std::endl << std::endl;
//
//  std::cout << "Footpath duration from CAZ to LIMA2: " << raptor.getStops().at("CAZ").getFootpaths().at("LIMA2").duration
//            << " seconds." << std::endl;
//  std::cout << "Footpath duration from LIMA2 to PSOC1: " << raptor.getStops().at("LIMA2").getFootpaths().at("PSOC1").duration
//            << " seconds." << std::endl;
//  std::cout << "Footpath duration from PSOC1 to CVLO: " << raptor.getStops().at("PSOC1").getFootpaths().at("CVLO").duration
//            << " seconds." << std::endl;
//  std::cout << "Sum: " << raptor.getStops().at("CAZ").getFootpaths().at("LIMA2").duration +
//                          raptor.getStops().at("LIMA2").getFootpaths().at("PSOC1").duration +
//                          raptor.getStops().at("PSOC1").getFootpaths().at("CVLO").duration << " seconds." << std::endl;
//  std::cout << "Footpath duration from CAZ to CVLO: "
//            << raptor.getStops().at("CAZ").getFootpaths().at("CVLO").duration << " seconds." << std::endl << std::endl;

}

void Application::showCommands() {
  std::cout << std::endl << "Available commands:" << std::endl;


  std::cout << std::left << std::setw(30) << " 1. query " << " Runs RAPTOR algorithm." << std::endl;
//  std::cout << "     example: query 5777 5776 22:00:00" << std::endl;
//  std::cout << "              query 5775 5813 03:00:33" << std::endl;
//  std::cout << "              query 5746 5756 12:22:33" << std::endl;
//  std::cout << "              query 5753 5782 19:44:00" << std::endl;
//  std::cout << "              query 5726 5739 06:44:00" << std::endl; // Metro Trindade to Lidador
//  std::cout << "              query SAL2 IPO5 14:00:00" << std::endl; // STCP Salgueiros to IPO
//  std::cout << "              query MAIA3 PARR3 5:55:55" << std::endl; // STCP Maia to Arrabida
  std::cout << std::left << std::setw(30) << " 2. help " << " Shows available commands. " << std::endl;
  std::cout << " 3. quit " << std::endl;
}
