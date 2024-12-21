/**
 * @file RaptorTests.cpp
 * @brief Unit tests for the RAPTOR algorithm implementation using Google Test framework.
 *
 * This file contains test cases to validate the functionality and correctness of the RAPTOR algorithm,
 * including edge cases, multimodal journeys, and handling of invalid inputs.
 *
 * @author Maria
 * @date 11/25/2024
 */

#include "gtest/gtest.h"
#include "./src/Raptor.h"

/**
 * @brief Loads data from GTFS files into in-memory data structures.
 *
 * @param inputDirectories A vector of strings containing paths to GTFS directories.
 * @param agencies A map to store agency data.
 * @param calendars A map to store calendar data.
 * @param trips A map to store trip data.
 * @param routes A map to store route data.
 * @param stops A map to store stop data.
 * @param stop_times A map to store stop time data.
 */
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

/**
 * @class RaptorTests
 * @brief A test suite for validating the RAPTOR algorithm.
 *
 * This test suite uses Google Test to verify the correctness of the RAPTOR algorithm
 * against various scenarios and edge cases.
 */
class RaptorTests : public ::testing::Test {
protected:
  static Raptor raptor; ///< Shared instance of the RAPTOR algorithm.

  /**
     * @brief Sets up data for all tests in the suite.
     *
     * Loads GTFS data from specified directories and initializes the RAPTOR algorithm.
     */
  static void SetUpTestSuite() {
    std::vector<std::string> inputDirectories = {std::string(DATASET_PATH) + "/Porto/stcp/GTFS/",
                                                 std::string(DATASET_PATH) + "/Porto/metro/GTFS/"};

    std::unordered_map<std::string, Agency> agencies;
    std::unordered_map<std::string, Calendar> calendars;
    std::unordered_map<std::string, Trip> trips;
    std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> routes;
    std::unordered_map<std::string, Stop> stops;
    std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> stop_times;

    loadData(inputDirectories, agencies, calendars, trips, routes, stops, stop_times);

    raptor = Raptor(agencies, calendars, stops, routes, trips, stop_times);
  }

};

Raptor RaptorTests::raptor;

/**
 * @test ValidJourneyQuery
 * @brief Tests a simple query for a valid journey between two stops.
 */
TEST_F(RaptorTests, ValidJourneyQuery) {
  Query query = {"5777", "5776", {2024, 10, 15}, {22, 30, 0}};

  raptor.setQuery(query);
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  ASSERT_TRUE(journeys.size() == 1);  // Ensure only one journey is found
  ASSERT_TRUE(raptor.isValidJourney(journeys.front()));
}

/**
 * @test from5777_toOTH2_day
 * @brief Tests a query for a journey during the day between stops 5777 and OTH2.
 */
TEST_F(RaptorTests, from5777_toOTH2_day) {
  Query query = {"5777", "OTH2", {2024, 10, 15}, {11, 30, 0}};

  raptor.setQuery(query);
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  for (auto &journey: journeys)
    ASSERT_TRUE(raptor.isValidJourney(journey));
}

/**
 * @test from5777_toOTH2_later
 * @brief Tests a query for a journey later in the evening between stops 5777 and OTH2.
 */
TEST_F(RaptorTests, from5777_toOTH2_later) {
  Query query = {"5777", "OTH2", {2024, 10, 15}, {22, 30, 0}};

  raptor.setQuery(query);
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  for (auto &journey: journeys)
    ASSERT_TRUE(raptor.isValidJourney(journey));
}

/**
 * @test from5777_toOTH2_night
 * @brief ests a query for a journey later in the night between stops 5777 and OTH2.
 */
TEST_F(RaptorTests, from5777_toOTH2_night) {
  Query query = {"5777", "OTH2", {2024, 10, 15}, {23, 35, 0}};

  raptor.setQuery(query);
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  for (auto &journey: journeys)
    ASSERT_TRUE(raptor.isValidJourney(journey));
}

/**
 * @test MultiDayJourney
 * @brief Tests a journey that spans multiple days.
 */
TEST_F(RaptorTests, MultiDayJourney) {
  Query query = {"TCRZ2", "SCT2", {2024, 12, 9}, {23, 40, 0}}; // Across days

  raptor.setQuery(query);
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  for (auto &journey: journeys)
    ASSERT_TRUE(raptor.isValidJourney(journey));

  ASSERT_EQ(journeys.back().arrival_day, Day::NextDay);  // Verify journey spans into the next day
}


/**
 * @test SameStartAndEndStop
 * @brief Tests a journey that starts and ends at the same stop.
 */
TEST_F(RaptorTests, SameStartAndEndStop) {
  Query query = {"5777", "5777", {2024, 12, 10}, {9, 0, 0}};

  raptor.setQuery(query);
  auto journeys = raptor.findJourneys();

  ASSERT_TRUE(journeys.empty());  // No journey should be found
}

/**
 * @test JourneySpanningWeekend
 * @brief Tests a journey that spans the weekend.
 */
TEST_F(RaptorTests, JourneySpanningWeekend) { // TODO: k=5, 7 steps
  Query query = {"TSL2", "LGEQ1", {2024, 12, 13}, {23, 0, 0}};  // Friday to Saturday

  raptor.setQuery(query);
  auto journeys = raptor.findJourneys();

  ASSERT_FALSE(journeys.empty());  // Ensure journeys are found
  for (auto &journey: journeys) {
    ASSERT_TRUE(raptor.isValidJourney(journey));  // Validate weekend service
  }
}

