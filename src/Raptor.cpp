
#include "Raptor.h"

Raptor::Raptor(const std::unordered_map<std::string, Agency> &agencies_,
               const std::unordered_map<std::string, Calendar> &calendars_,
               const std::unordered_map<std::string, Stop> &stops,
               const std::unordered_map<std::pair<std::string, std::string>, Route, pair_hash> &routes,
               const std::unordered_map<std::string, Trip> &trips,
               const std::unordered_map<std::pair<std::string, std::string>, StopTime, pair_hash> &stop_times)
        : stops_(stops), routes_(routes), trips_(trips), stop_times_(stop_times) {
  k = 1;
}

void Raptor::setQuery(const Query &query) {
  query_ = query;
}

std::vector<std::vector<JourneyStep>> Raptor::findJourneys() {
  std::vector<std::vector<JourneyStep>> journeys;

  initializeAlgorithm();

  while (true) {

    // 1st: set an upper bound for the current round
    for (const auto &[id, stop]: stops_) {
      min_arrival_time[id].push_back(min_arrival_time[id][k - 1]);
    }

    prev_marked_stops = marked_stops;
    marked_stops.clear();

    // Accumulate routes serving marked stops from previous round
    // ((route_id, direction_id), stop_id)
    std::unordered_set<std::pair<std::pair<std::string, std::string>, std::string>, nested_pair_hash> routes_stops_set = accumulateRoutesServingStops();

    // 2nd: Traverse each route
    traverseRoutes(routes_stops_set);

    // Look for footpaths
    handleFootpaths();

//    showMinArrivalTimes();

    std::cout << "Marked " << std::setw(2) << marked_stops.size() << " stops during round " << k << std::endl;
    // Stopping criterion: if no stops are marked, then stop
    if (marked_stops.empty()) break;

    if (marked_stops.find(query_.target_id) != marked_stops.end()) {
      std::vector<JourneyStep> journey = reconstructJourney();

      if (!journey.empty() && journey.front().src_stop->getField("stop_id") == query_.source_id) {
        journeys.push_back(journey);
        int ntrips = 0;

        for (const JourneyStep &step: journey)
          if (step.trip_id.has_value()) ntrips++;

        std::cout << "Found journey with " << ntrips << " trips and " << journey.size() << " steps." << std::endl;
        Raptor::showJourney(journey);
      }
    }

    k++;
  }

  return journeys;
}

void Raptor::initializeAlgorithm() {
  std::cout << "Finding journeys from " << stops_[query_.source_id].getField("stop_name") << " to "
            << stops_[query_.target_id].getField("stop_name")
            << " departing at " << query_.departure_time << std::endl << std::endl;

  min_arrival_time.clear();
  prev_marked_stops.clear();
  marked_stops.clear();

  for (const auto &[id, stop]: stops_)
    min_arrival_time[id] = std::vector<StopInfo>(1, {INF, std::nullopt, std::nullopt});

  min_arrival_time[query_.source_id][0].min_arrival_time = Utils::timeToSeconds(query_.departure_time);
  marked_stops.insert(query_.source_id);

  k = 1;
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
      for (Stop *stop: routes_[route_key].getStops()) {
        std::string stop_id = stop->getField("stop_id");

        // Check if a point p' is already in the list
        auto existing_entry = routes_stops_set.find({route_key, stop_id});
        if (existing_entry != routes_stops_set.end()) {
          already_has_point = true;

          auto it_marked = std::find_if(routes_[route_key].getStops().begin(), routes_[route_key].getStops().end(),
                                        [&](Stop *s) { return s->getField("stop_id") == marked_stop_id; });
          auto it_stop = std::find_if(routes_[route_key].getStops().begin(), routes_[route_key].getStops().end(),
                                      [&](Stop *s) { return s->getField("stop_id") == stop_id; });

          if (it_marked < it_stop) { // if marked_p comes before p'
            routes_stops_set.erase(existing_entry);
            routes_stops_set.insert({route_key, marked_stop_id});
          } // else (if marked_p does not come before p'), we do not add it to the set and leave the p' entry
        }
      }

      if (!already_has_point)  // The route does not have a point in the list
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

    auto stop_it = std::find_if(route.getStops().begin(), route.getStops().end(),
                                [&](Stop *stop) { return stop->getField("stop_id") == p_stop_id; });

    // For each stop pi on this route, try to find the earliest trip (et) that can be taken
    for (auto it = stop_it; it != route.getStops().end(); ++it) {
      Stop *pi_stop = *it;
      std::string pi_stop_id = pi_stop->getField("stop_id");

      auto et_id = findEarliestTrip(pi_stop_id, route_key);
      if (et_id.has_value()) {
        traverseTrip(et_id.value(), pi_stop_id);
      } else continue; // No valid trip found for this stop

    } // end each stop pi on route

    route_stop = routes_stops_set.erase(route_stop);
  } // end each route

}

std::optional<std::string>
Raptor::findEarliestTrip(const std::string &pi_stop_id, const std::pair<std::string, std::string> &route_key) {

  // Find the earliest trip in route r that can be caught at stop pi in round k
  for (StopTime *stop_time: stops_[pi_stop_id].getStopTimes()) {

    // Check if the stop_time is from a trip that belongs to the route being traversed
    std::string trip_id = stop_time->getField("trip_id");

    if (isValidTrip(trip_id, route_key, stop_time))
      return stop_time->getField("trip_id"); // We can return because stop_times is ordered
  }

  return std::nullopt;
}

bool Raptor::isValidTrip(const std::string &trip_id, const std::pair<std::string, std::string> &route_key,
                         StopTime *stop_time) {

  const Trip &trip = trips_[trip_id];
  return ((trip.getField("route_id") == route_key.first) &&
          (trip.getField("direction_id") == route_key.second) &&
          (Utils::timeToSeconds(stop_time->getField("departure_time")) >=
           min_arrival_time[stop_time->getField("stop_id")][k - 1].min_arrival_time) &&
          (Utils::timeToSeconds(stop_time->getField("departure_time")) <
           min_arrival_time[query_.target_id][k].min_arrival_time));
}

void Raptor::traverseTrip(std::string &et_id, std::string &pi_stop_id) {
  Trip et = trips_[et_id];

  auto et_stop_it = std::find_if(et.getStopTimes().begin(), et.getStopTimes().end(),
                                 [&](StopTime *stoptime) { return stoptime->getField("stop_id") == pi_stop_id; });

  // Traverse remaining stops on the trip to update arrival times
  for (auto next_stop_time = std::next(et_stop_it); next_stop_time != et.getStopTimes().end(); ++next_stop_time) {

    std::string next_stop_id = (*next_stop_time)->getField("stop_id");

    // Access arrival time at next_stop_id for trip et_id
    int arrival_time = Utils::timeToSeconds((*next_stop_time)->getField("arrival_time"));

    // If arrival time can be improved, update Tk(pj) using et
    if (arrival_time < std::min(min_arrival_time[next_stop_id][k].min_arrival_time,
                                min_arrival_time[query_.target_id][k].min_arrival_time)) {
//      if ((next_stop_id == "IPO4") || (next_stop_id == "HSJ7")|| (next_stop_id == "HSJ2")
//       || (next_stop_id == "HSJ9") || (next_stop_id == "HSJ11")|| (next_stop_id == "ARS4"))
//      if ((next_stop_id == "PRU3") || (next_stop_id == "GATS2")|| (next_stop_id == "VIS1")
//          || (next_stop_id == "FG2") )
//
//        std::cout << "Marking " << std::setw(10) << next_stop_id << et_id << " from " << std::setw(10) << pi_stop_id << " " << Utils::secondsToTime(arrival_time)
//                << " < min(" << Utils::secondsToTime(min_arrival_time[next_stop_id][k].min_arrival_time) << ", " << Utils::secondsToTime(min_arrival_time[query_.target_id][k].min_arrival_time) << ")" << std::endl;
      min_arrival_time[next_stop_id][k] = {arrival_time, et_id, pi_stop_id};
      marked_stops.insert(next_stop_id); // Mark this stop for the next round
    }

    // Check if an earlier trip can be caught at stop i (because a quicker path was found in a previous round)
    if ((min_arrival_time[next_stop_id][k - 1].parent_trip_id.has_value()) // Because we can instantly arrive at source
        && (min_arrival_time[next_stop_id][k - 1].min_arrival_time < arrival_time))  // if Tk-1(pi) < Tarr(t, pi)
      break;

  } // end remaining stops on trip et_id

}

void Raptor::handleFootpaths() {
  // For each previously marked stop p
  for (const auto &stop_id: prev_marked_stops) {
    int p_prev_arrival = min_arrival_time[stop_id][k-1].min_arrival_time;
    // For each footpath (p, p')
    for (const auto &[dest_id, footpath]: stops_[stop_id].getFootpaths()) {
      int new_arrival = p_prev_arrival + footpath.duration;
      if (new_arrival < std::min(min_arrival_time[dest_id][k].min_arrival_time,
                                 min_arrival_time[query_.target_id][k].min_arrival_time)) {
        // Only updates footpath if it goes to a stop that has not made any improvements
//        if ((dest_id == "IPO4") || (dest_id == "HSJ7") || (dest_id == "HSJ2")
//            || (dest_id == "HSJ9")|| (dest_id == "HSJ11") || (dest_id == "ARS4"))
//        if ((dest_id == "PRU3") || (dest_id == "GATS2")|| (dest_id == "VIS1")
//            || (dest_id == "FG2") )
//
//          std::cout << "Marking " << std::setw(10) << dest_id << "fp from " << std::setw(10) << stop_id
//                  << " " << Utils::secondsToTime(min_arrival_time[stop_id][k].min_arrival_time) << " + " << Utils::secondsToTime(footpath.duration) << " = " << Utils::secondsToTime(new_arrival)
//                  << " < min(" << Utils::secondsToTime(min_arrival_time[dest_id][k].min_arrival_time) << ", " << Utils::secondsToTime(min_arrival_time[query_.target_id][k].min_arrival_time) << ")" << std::endl;
        min_arrival_time[dest_id][k] = {new_arrival, std::nullopt, stop_id};
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
      departure_time = min_arrival_time[parent_stop_id][k].min_arrival_time;
      duration = stops_[parent_stop_id].getFootpaths().at(current_stop_id).duration;
      arrival_time = departure_time + duration;
    } else { // Trip
      const std::string &parent_trip_id = parent_trip_id_opt.value();
      departure_time = Utils::timeToSeconds(stop_times_[{parent_trip_id, parent_stop_id}].getField("departure_time"));
      arrival_time = min_arrival_time[current_stop_id][k].min_arrival_time;
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

void Raptor::showMinArrivalTimes() {
  std::cout << std::endl << "    Minimal arrival times:" << std::endl << std::endl;

  // Print the header row
  std::cout << std::setw(6) << "Stop" << std::setw(28) << "Stop name";
  for (int current_k = 0; current_k <= k; current_k++)
    std::cout << std::setw(10) << current_k;

  std::cout << std::endl;

  // Print the arrival times for each stop
  for (const auto &[stop_id, arrivals]: min_arrival_time) {
    std::cout << std::setw(6) << stop_id;
    std::cout << std::setw(28) << stops_[stop_id].getField("stop_name");

    for (const StopInfo &arrival: arrivals)
      std::cout << std::setw(10) << Utils::secondsToTime(arrival.min_arrival_time);

    std::cout << std::endl;
  }
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
