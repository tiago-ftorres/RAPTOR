
#include "Raptor.h"

#include <utility>


Raptor::Raptor(const std::unordered_map<std::string, Agency> &agencies,
               const std::unordered_map<std::string, Calendar> &calendars,
               const std::unordered_map<std::string, Stop> &stops,
               const std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> &routes,
               const std::unordered_map<std::string, Trip> &trips,
               const std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> &stop_times)
        : agencies_(agencies), calendars_(calendars), stops_(stops), routes_(routes), trips_(trips),
          stop_times_(stop_times) {
  k = 1;

  std::cout << "Raptor initialized with "
            << agencies_.size() << " agencies, "
            << calendars_.size() << " calendars, "
            << stops.size() << " stops, "
            << routes.size() << " routes, "
            << trips.size() << " trips and "
            << stop_times.size() << " stop times." << std::endl;

  initializeFootpaths();
}

void Raptor::setQuery(const Query &query) {
  query_ = query;
}

void Raptor::initializeFootpaths() {
  // Initialize footpaths
  std::cout << "Initializing footpaths..." << std::endl;
  auto start_time = std::chrono::high_resolution_clock::now();

  // Avoid duplicating calculations for both sides
  for (auto it1 = stops_.begin(); it1 != stops_.end(); ++it1) {
    const std::string &id1 = it1->first;
    Stop &stop1 = it1->second;

    // Start the inner loop from the next element
    for (auto it2 = std::next(it1); it2 != stops_.end(); ++it2) {
      const std::string &id2 = it2->first; // stop_id
      Stop &stop2 = it2->second; // stop itself

      // Calculate duration between the two stops
      int duration = Utils::getDuration(
              stop1.getField("stop_lat"), stop1.getField("stop_lon"),
              stop2.getField("stop_lat"), stop2.getField("stop_lon"));

      // Add footpaths in both directions
      stop1.addFootpath(id2, duration);
      stop2.addFootpath(id1, duration);
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

  std::cout << "Footpaths initialized in " << duration << " ms ("
            << duration / 1000 << " seconds)." << std::endl;
}

void Raptor::initializeAlgorithm() {
  std::ostringstream time_oss;
  time_oss << std::setw(2) << std::setfill('0') << query_.departure_time.hours << ":"
           << std::setw(2) << std::setfill('0') << query_.departure_time.minutes << ":00";

  std::cout << "Finding journeys from " << stops_[query_.source_id].getField("stop_name")
            << " to " << stops_[query_.target_id].getField("stop_name")
            << " departing " << query_.date.day << "/" << query_.date.month << "/" << query_.date.year
            << " at " << time_oss.str() << std::endl << std::endl;

  min_arrival_time.clear();
  prev_marked_stops.clear();
  marked_stops.clear();

  for (const auto &[id, stop]: stops_)
    min_arrival_time[id] = std::vector<StopInfo>(1, {std::nullopt, std::nullopt, std::nullopt, std::nullopt});

  k = 0;
  setMinArrivalTime(query_.source_id,
                    {Utils::timeToSeconds(query_.departure_time), std::nullopt, std::nullopt, Day::CurrentDay});
  marked_stops.insert(query_.source_id);

  k++; // k=1

  days_active_trips_ids_[Day::CurrentDay] = std::unordered_set<std::string>();
  days_active_trips_ids_[Day::NextDay] = std::unordered_set<std::string>();

  fillActiveTrips(Day::CurrentDay);
}

void Raptor::setMinArrivalTime(const std::string &stop_id, StopInfo stop_info) {

  if (min_arrival_time[stop_id].size() <= k)
    min_arrival_time[stop_id].resize(k + 1);

  min_arrival_time[stop_id][k] = std::move(stop_info);

  // In case it passed midnight, fill tomorrow's active trips
  std::optional<Day> day = stop_info.day;
  if (day.has_value() && (day.value() == Day::NextDay) && (days_active_trips_ids_[Day::NextDay].empty()))
    fillActiveTrips(Day::NextDay);
}

void Raptor::fillActiveTrips(Day day) {
  Date target_date = (day == Day::CurrentDay) ? query_.date : Utils::addOneDay(query_.date);

  // Iterates over all trips
  for (auto &[trip_id, trip]: trips_) {
    const Calendar &calendar = calendars_.at(trip.getField("service_id"));

    if (isServiceActive(calendar, target_date))
      days_active_trips_ids_[day].insert(trip_id);

  }
}

std::vector<std::vector<JourneyStep>> Raptor::findJourneys() {
  std::vector<std::vector<JourneyStep>> journeys;

  initializeAlgorithm();

  while (true) {

    // 1st: set an upper bound for the current round
    for (const auto &[stop_id, stop]: stops_)
      setMinArrivalTime(stop_id, min_arrival_time[stop_id][k - 1]);

    prev_marked_stops = marked_stops;
    marked_stops.clear();

    std::cout << "There are " << prev_marked_stops.size() << " previously marked stops." << std::endl;

    // Accumulate routes serving marked stops from previous round
    // ((route_id, direction_id), stop_id)
    std::unordered_set<std::pair<std::pair<std::string, std::string>, std::string>, nested_pair_hash> routes_stops_set = accumulateRoutesServingStops();
    std::cout << "Accumulated " << routes_stops_set.size() << " routes serving stops." << std::endl;

    // 2nd: Traverse each route
    traverseRoutes(routes_stops_set);
    std::cout << "Traversed routes. " << marked_stops.size() << " stops are marked." << std::endl;

    // Look for footpaths
    handleFootpaths();
    std::cout << "Handled footpaths. " << marked_stops.size() << " stops are marked." << std::endl;

    // Stopping criterion: if no stops are marked, then stop
    if (marked_stops.empty()) break;

    if (marked_stops.find(query_.target_id) != marked_stops.end()) {
      std::cout << "Target stop was marked. Reconstructing journey..." << std::endl;

      std::vector<JourneyStep> journey = reconstructJourney();

      if (isValidJourney(journey)) {
        journeys.push_back(journey);

        std::cout << "Found journey with " << journey.size() << " step(s)." << std::endl;
        Raptor::showJourney(journey);
      }
    } else {
      std::cout << "Target stop was not marked." << std::endl;
    }

    k++;
  }

  return journeys;
}

std::unordered_set<std::pair<std::pair<std::string, std::string>, std::string>, nested_pair_hash>
Raptor::accumulateRoutesServingStops() {
  std::unordered_set<std::pair<std::pair<std::string, std::string>, std::string>, nested_pair_hash> routes_stops_set;

  // For each previously marked stop p
  for (const auto &marked_stop_id: prev_marked_stops) {

    // For each route r serving p
    for (const auto &route_key: stops_[marked_stop_id].getRouteKeys()) {

      // Iterate over all stops in the route
      bool already_has_point = false;
      for (std::string stop_id: routes_[route_key].getStopsIds()) {

        // Check if a point p' is already in the list
        auto existing_entry = routes_stops_set.find({route_key, stop_id});
        if (existing_entry != routes_stops_set.end()) {
          already_has_point = true;

          auto it_marked = std::find_if(routes_[route_key].getStopsIds().begin(),
                                        routes_[route_key].getStopsIds().end(),
                                        [&](const std::string &s_id) { return s_id == marked_stop_id; });
          auto it_stop = std::find_if(routes_[route_key].getStopsIds().begin(), routes_[route_key].getStopsIds().end(),
                                      [&](const std::string &s_id) { return s_id == stop_id; });

          if (it_marked < it_stop) { // if marked_p comes before p'
            routes_stops_set.erase(existing_entry);
            routes_stops_set.insert({route_key, marked_stop_id});
          } // else (if marked_p does not come before p'), we do not add it to the set and leave the p' entry
        }
      }

      if (!already_has_point)   // The route does not have a point in the list
        routes_stops_set.insert({route_key, marked_stop_id});
    }
  }

  return routes_stops_set;
}

void Raptor::traverseRoutes(
        std::unordered_set<std::pair<std::pair<std::string, std::string>, std::string>, nested_pair_hash> routes_stops_set) {
  auto route_stop = routes_stops_set.begin();

  while (route_stop != routes_stops_set.end()) {
    const auto &[route_key, p_stop_id] = *route_stop;
    const Route &route = routes_[route_key];

    auto stop_it = std::find_if(route.getStopsIds().begin(), route.getStopsIds().end(),
                                [&](const std::string &s_id) {
                                  return s_id == p_stop_id;
                                });

    // For each stop pi on this route, try to find the earliest trip (et) that can be taken
    for (auto it = stop_it; it != route.getStopsIds().end(); ++it) {
      std::string pi_stop_id = *it;

      // Find the earliest trip in route r that can be caught at stop pi in round k
      auto et_id = findEarliestTrip(pi_stop_id, route_key);

      // If a valid trip was found, traverse the trip
      if (et_id.has_value())
        traverseTrip(et_id.value(), pi_stop_id);
      else continue; // No valid trip found for this stop

    } // end each stop pi on route

    route_stop = routes_stops_set.erase(route_stop);
  } // end each route

}

std::optional<std::string>
Raptor::findEarliestTrip(const std::string &pi_stop_id, const std::pair<std::string, std::string> &route_key) {

  // Find the earliest trip in route r that can be caught at stop pi in round k
  for (const auto &stop_time_key: stops_[pi_stop_id].getStopTimesKeys()) {
    const StopTime &stop_time = stop_times_.at(stop_time_key);

    if (isValidTrip(route_key, stop_time))
      return stop_time.getField("trip_id"); // We can return because stop_times is ordered
  }

  return std::nullopt;
}

bool Raptor::isValidTrip(const std::pair<std::string, std::string> &route_key,
                         const StopTime &stop_time) {

  const std::string &trip_id = stop_time.getField("trip_id");
  std::optional<Day> stop_day = min_arrival_time[stop_time.getField("stop_id")][k - 1].day;

  // If stop is still not reachable or if is today reachable, but trip is neither today nor tomorrow active
  if ((!stop_day.has_value() || stop_day.value() == Day::CurrentDay)
      && !isTripActive(trip_id, Day::CurrentDay)
      && !isTripActive(trip_id, Day::NextDay))
    return false;

  // If stop is reachable tomorrow, but trip is not tomorrow active
  if (stop_day.has_value()
      && stop_day.value() == Day::NextDay
      && (!isTripActive(trip_id, Day::NextDay)))
    return false;
  const Trip &trip = trips_[trip_id];
  auto [route_id, direction_id] = route_key;

  // Check if the stop_time is from a trip that belongs to the route being traversed
  if ((trip.getField("route_id") != route_id) || (trip.getField("direction_id") != direction_id))
    return false;

  // Check if the time is valid
  int departure_time_seconds = Utils::timeToSeconds(stop_time.getField("departure_time"));
  if (departure_time_seconds < min_arrival_time[stop_time.getField("stop_id")][k - 1].min_arrival_time || // Required
      departure_time_seconds >= min_arrival_time[query_.target_id][k].min_arrival_time)  // Pruning
    return false;

  return true;
}

bool Raptor::isTripActive(const std::string &trip_id, Day day) {
  return days_active_trips_ids_[day].find(trip_id) != days_active_trips_ids_[day].end();
}

bool Raptor::isServiceActive(const Calendar &calendar, const Date &date) const {

  // Check if the date is within the calendar's start and end dates
  if (!Utils::dateWithinRange(date, calendar.getField("start_date"), calendar.getField("end_date")))
    return false;

  // Check if the day of the week is active
  std::tm time_info = {};
  time_info.tm_year = query_.date.year - 1900;
  time_info.tm_mon = query_.date.month - 1;
  time_info.tm_mday = query_.date.day;
  std::mktime(&time_info);
  int weekday = time_info.tm_wday; // 0 = sunday, 1 = monday, etc.

  switch (weekday) {
    case 0:
      return std::stoi(calendar.getField("sunday"));
    case 1:
      return std::stoi(calendar.getField("monday"));
    case 2:
      return std::stoi(calendar.getField("tuesday"));
    case 3:
      return std::stoi(calendar.getField("wednesday"));
    case 4:
      return std::stoi(calendar.getField("thursday"));
    case 5:
      return std::stoi(calendar.getField("friday"));
    case 6:
      return std::stoi(calendar.getField("saturday"));
    default:
      return false;
  }
}

// TODO: create a struct for stop_time_key
// TODO: use .at() instead of []
void Raptor::traverseTrip(std::string &et_id, std::string &pi_stop_id) {
  Trip et = trips_[et_id];

  auto et_stop_it = std::find_if(et.getStopTimesKeys().begin(), et.getStopTimesKeys().end(),
                                 [&](const std::pair<std::string, std::string> &st_key) {
                                   return st_key.second == pi_stop_id;
                                 });

  // Traverse remaining stops on the trip to update arrival times
  for (auto next_stop_time_key = std::next(et_stop_it);
       next_stop_time_key != et.getStopTimesKeys().end(); ++next_stop_time_key) {

    auto [trip_id, next_stop_id] = *next_stop_time_key;
    StopTime &next_stop_time = stop_times_[*next_stop_time_key];

    // Access arrival time at next_stop_id for trip et_id
    int arrival_time = Utils::timeToSeconds(next_stop_time.getField("arrival_time"));

    // If arrival time can be improved, update Tk(pj) using et
    if (arrivesEarlier(arrival_time, min_arrival_time[next_stop_id][k].min_arrival_time)
        && arrivesEarlier(arrival_time, min_arrival_time[query_.target_id][k].min_arrival_time)) { // Pruning

//        std::cout << "Marking " << std::setw(10) << next_stop_id << et_id << " from " << std::setw(10) << pi_stop_id << " " << Utils::secondsToTime(arrival_time)
//                << " < min(" << Utils::secondsToTime(min_arrival_time[next_stop_id][k].min_arrival_time) << ", " << Utils::secondsToTime(min_arrival_time[query_.target_id][k].min_arrival_time) << ")" << std::endl;
      Day day = arrival_time > MIDNIGHT ? Day::NextDay : Day::CurrentDay;
      setMinArrivalTime(next_stop_id, {arrival_time, et_id, pi_stop_id, day});

      marked_stops.insert(next_stop_id); // Mark this stop for the next round
    }

    // Check if an earlier trip can be caught at stop i (because a quicker path was found in a previous round)
    if ((min_arrival_time[next_stop_id][k - 1].parent_trip_id.has_value()) // Because we can instantly arrive at source
        && (min_arrival_time[next_stop_id][k - 1].min_arrival_time < arrival_time))  // if Tk-1(pi) < Tarr(t, pi)
      break;

  } // end remaining stops on trip et_id

}

bool Raptor::arrivesEarlier(int secondsA, std::optional<int> secondsB) {
  if (!secondsB.has_value()) return true; // if still not set, then any value is better
  return secondsA < secondsB.value();
}

void Raptor::handleFootpaths() {
  // For each previously marked stop p
  for (const auto &stop_id: prev_marked_stops) {
    std::optional<int> p_prev_arrival = min_arrival_time[stop_id][k - 1].min_arrival_time;
    // For each footpath (p, p')
    for (const auto &[dest_id, footpath]: stops_[stop_id].getFootpaths()) {
      int new_arrival = p_prev_arrival.value() + footpath.duration;

      if (arrivesEarlier(new_arrival, min_arrival_time[dest_id][k].min_arrival_time)
          && arrivesEarlier(new_arrival, min_arrival_time[query_.target_id][k].min_arrival_time)) {
        // Only updates footpath if it goes to a stop that has not made any improvements
//        std::cout << "Marking " << std::setw(10) << dest_id << "fp from " << std::setw(10) << stop_id
//                  << " " << Utils::secondsToTime(min_arrival_time[stop_id][k].min_arrival_time) << " + "
//                  << Utils::secondsToTime(footpath.duration) << " = " << Utils::secondsToTime(new_arrival)
//                  << " < min(" << Utils::secondsToTime(min_arrival_time[dest_id][k].min_arrival_time) << ", "
//                  << Utils::secondsToTime(min_arrival_time[query_.target_id][k].min_arrival_time) << ")" << std::endl;
        Day day = new_arrival > MIDNIGHT ? Day::NextDay : Day::CurrentDay;
        setMinArrivalTime(dest_id, {new_arrival, std::nullopt, stop_id, day});

        marked_stops.insert(dest_id);
      }

    } // end each footpath (p, p')
  } // end each marked stop p

}

std::vector<JourneyStep> Raptor::reconstructJourney() {
  std::vector<JourneyStep> journey;
  std::string current_stop_id = query_.target_id;

  std::cout << "Reconstructing journey at k " << k << std::endl;

  while (true) {

    const std::optional<std::string> parent_trip_id_opt = min_arrival_time[current_stop_id][k].parent_trip_id;
    const std::optional<std::string> parent_stop_id_opt = min_arrival_time[current_stop_id][k].parent_stop_id;

    if (!parent_stop_id_opt.has_value()) break;

    const std::string &parent_stop_id = parent_stop_id_opt.value();

    int departure_time, duration, arrival_time;
    if (!parent_trip_id_opt.has_value()) { // Footpath
      departure_time = min_arrival_time[parent_stop_id][k].min_arrival_time.value();
      duration = stops_[parent_stop_id].getFootpaths().at(current_stop_id).duration;
      arrival_time = departure_time + duration;
    } else { // Trip
      const std::string &parent_trip_id = parent_trip_id_opt.value();
      departure_time = Utils::timeToSeconds(stop_times_[{parent_trip_id, parent_stop_id}].getField("departure_time"));
      arrival_time = min_arrival_time[current_stop_id][k].min_arrival_time.value();
      duration = arrival_time - departure_time;
    }

    JourneyStep step = {parent_trip_id_opt, &stops_[parent_stop_id], &stops_[current_stop_id],
                        departure_time, duration, arrival_time};
    journey.push_back(step);

    // Update to the previous stop boarded
    current_stop_id = parent_stop_id;

    // Make sure we only add valid journeys
    if (current_stop_id == query_.source_id) break;
  }
  // Reverse the journey to obtain the correct sequence
  std::reverse(journey.begin(), journey.end());

  return journey;
}

bool Raptor::isValidJourney(std::vector<JourneyStep> journey) const {
  if (journey.empty() ||
      (journey.front().src_stop->getField("stop_id") != query_.source_id))
    return false;

  for (size_t i = 0; i < journey.size() - 1; ++i)
    if (!journey[i].trip_id.has_value() && !journey[i + 1].trip_id.has_value())
      return false;

  return true;
}

const std::unordered_map<std::string, Stop> &Raptor::getStops() const {
  return stops_;
}

void Raptor::showJourney(const std::vector<JourneyStep> &journey) {

  // Print the header row
  std::cout << std::setw(5) << "step" << std::setw(13) << " trip "
            << std::setw(8) << "stop " << std::setw(15) << "(name)" << std::setw(10) << "dep_time "
            << std::setw(10) << "duration "
            << std::setw(14) << "-> stop " << std::setw(15) << "(name)" << std::setw(9) << "arr_time " << std::endl;

  // Print the journey steps
  for (int j = 0; j < journey.size(); j++) {
    const JourneyStep &step = journey[j];
    std::cout << std::setw(6) << j + 1;

    if (step.trip_id.has_value())
      std::cout << std::setw(12) << step.trip_id.value();
    else
      std::cout << std::setw(12) << "footpath";

    std::cout << std::setw(8) << step.src_stop->getField("stop_id") << std::setw(15)
              << Utils::getFirstWord(step.src_stop->getField("stop_name")) << std::setw(10)
              << Utils::secondsToTime(step.departure_time)
              << std::setw(10) << Utils::secondsToTime(step.duration)
              << std::setw(14) << step.dest_stop->getField("stop_id") << std::setw(15)
              << Utils::getFirstWord(step.dest_stop->getField("stop_name")) << std::setw(9)
              << Utils::secondsToTime(step.arrival_time);

    std::cout << std::endl << std::endl;
  }

}


