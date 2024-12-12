//
// Created by maria on 11/25/2024.
//

#include "gtest/gtest.h"
#include "./src/Raptor.h"
#include "./src/NetworkObjects/DataStructures.h"
#include "./src/Utils.h"
#include "./src/Parser.h"

void loadData(
        const std::vector<std::string> &inputDirectories,
        std::unordered_map<std::string, Agency> &agencies,
        std::unordered_map<std::string, Calendar> &calendars,
        std::unordered_map<std::string, Trip> &trips,
        std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> &routes,
        std::unordered_map<std::string, Stop> &stops,
        std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> &stop_times) {

  for (const auto &dir: inputDirectories) {

    Parser parser(dir);
    auto dirAgencies = parser.getAgencies();
    agencies.insert(dirAgencies.begin(), dirAgencies.end());
    auto dirCalendars = parser.getCalendars();
    calendars.insert(dirCalendars.begin(), dirCalendars.end());
    auto dirTrips = parser.getTrips();
    trips.insert(dirTrips.begin(), dirTrips.end());
    auto dirRoutes = parser.getRoutes();
    routes.insert(dirRoutes.begin(), dirRoutes.end());
    auto dirStops = parser.getStops();
    stops.insert(dirStops.begin(), dirStops.end());
    auto dirStopTimes = parser.getStopTimes();
    stop_times.insert(dirStopTimes.begin(), dirStopTimes.end());
  }
}


class RaptorTests : public ::testing::Test {
protected:
  // This will hold the Raptor instance
  Raptor raptor;

  // This will be run before each test
  void SetUp() override {
    // Initialize the input directories and query
    std::vector<std::string> inputDirectories = {std::string(DATASET_PATH) + "/Porto/stcp/GTFS/",
                                                 std::string(DATASET_PATH) + "/Porto/metro/GTFS/"};

    std::unordered_map<std::string, Agency> agencies;
    std::unordered_map<std::string, Calendar> calendars;
    std::unordered_map<std::string, Trip> trips;
    std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> routes;
    std::unordered_map<std::string, Stop> stops;
    std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> stop_times;

    loadData(inputDirectories, agencies, calendars, trips, routes, stops, stop_times);

    Raptor raptor(agencies, calendars, stops, routes, trips, stop_times);

  }

  // This can be overridden if any teardown/cleanup is needed after tests
  void TearDown() override {
    // Any necessary cleanup here
  }
};

// Example Test Case 1
TEST_F(RaptorTests, ValidJourneyQuery) {
  Query query = {"5777", "5776", {2024, 10, 15}, {22, 30, 0}}; // Source, Target, Date, Departure Time

  raptor.setQuery(query);  // Set a new query
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  for (auto &journey: journeys) {
    ASSERT_TRUE(raptor.isValidJourney(journey));  // Verify the journey is valid
  }
}

// Example Test Case 2
TEST_F(RaptorTests, from5777_toOTH2_day) {
  Query query = {"5777", "OTH2", {2024, 10, 15}, {11, 30, 0}}; // Source, Target, Date, Departure Time

  raptor.setQuery(query);  // Set a new query
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  for (auto &journey: journeys) {
    ASSERT_TRUE(raptor.isValidJourney(journey));  // Verify the journey is valid
  }
}

// Example Test Case 3
TEST_F(RaptorTests, from5777_toOTH2_night) {
  Query query = {"5777", "OTH2", {2024, 10, 15}, {22, 30, 0}}; // Source, Target, Date, Departure Time

  raptor.setQuery(query);  // Set a new query
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  for (auto &journey: journeys) {
    ASSERT_TRUE(raptor.isValidJourney(journey));  // Verify the journey is valid
  }
}

// Example Test Case 4
TEST_F(RaptorTests, MultiDayJourney) {
  Query query = {"TCRZ2", "SCT2", {2024, 12, 9}, {23, 40, 0}}; // Across days

  raptor.setQuery(query);  // Set a new query
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  for (auto &journey: journeys) {
    ASSERT_TRUE(raptor.isValidJourney(journey));  // Verify the journey is valid
  }

  ASSERT_EQ(journeys.back().arrival_day, Day::NextDay);  // Verify journey spans into the next day
}
