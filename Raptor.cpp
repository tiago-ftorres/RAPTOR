#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "Raptor.h"

Raptor::Raptor(const std::unordered_map<int, Stop>& stops,
               const std::unordered_map<int, Route>& routes,
               const std::unordered_map<int, Trip>& trips,
               const std::vector<StopTime>& stop_times) // TODO: primary key to be (trip_id, stop_sequence)
                : stops_(stops), routes_(routes), trips_(trips), stop_times_(stop_times) {
  initializeData();
}

void Raptor::initializeData() {
  // Initialize footpaths
  for (auto& [id, stop] : stops_) {
    for (const auto& [other_id, other_stop]: stops_) {
      if (id != other_id) {
        stop.footpaths[other_id] = {other_id, getDuration(stop.coordinates, other_stop.coordinates)};
      }
    }
  }

  // Associate stop_times to trips
  for (auto& stop_time : stop_times_) {
    trips_[stop_time.trip_id].stop_times.push_back(&stop_time);
  }

  // Order each trip's stop_times
  for (auto& [id, trip] : trips_) {
    std::sort(trip.stop_times.begin(), trip.stop_times.end(), [](const StopTime* a, const StopTime* b) {
      return a->stop_sequence < b->stop_sequence;
    });
  }

  // Associate trips to routes
  for (auto& [id, trip] : trips_){
    routes_[trip.route_id].trips.push_back(&trip);
  }

  // Order each route's trip by earliest to latest
  for (auto& [id, route] : routes_ ) {
    std::sort(route.trips.begin(), route.trips.end(), [&](const Trip *a, const Trip *b) {
      return timeToSeconds(a->stop_times.front()->arrival_time) < timeToSeconds(b->stop_times.front()->arrival_time);
    });
  }

  // Associate stops to routes
  for (auto& [id, route] : routes_){
    // If there is no trip associated to this route, skip the route
    if (route.trips.empty()) continue;

    // A route stops' order will be the same as any of the routes' trip's stops' order
    for (const auto& stop_time: route.trips[0]->stop_times){
      route.stops.push_back(&stops_[stop_time->stop_id]);
    }
  }

  for (StopTime& stop_time : stop_times_){
    std::cout << "Associating stop_time for trip " << stop_time.trip_id << " and stop " << stop_time.stop_id << std::endl;
    // Associate stop_times to stops
    stops_[stop_time.stop_id].stop_times.push_back(&stop_time);

    // Associate routes_ids to stops
    stops_[stop_time.stop_id].routes_ids.push_back(trips_[stop_time.trip_id].route_id);
  }

  for (auto& [id, stop] : stops_){
    std::cout << "Stop " << id << " has " << stop.stop_times.size() << " stop_times."<< std::endl;
  }

  // Order each stop's stop_times by earliest to latest departure time
  for (auto& [id, stop] : stops_){
    std::sort(stop.stop_times.begin(), stop.stop_times.end(), [&](const StopTime *a, const StopTime *b) {
      return timeToSeconds(a->departure_time) < timeToSeconds(b->departure_time);
    });
  }

}

std::vector<std::vector<JourneyStep>> Raptor::findRoute(const Query &query) {
  std::unordered_set<int> marked_stops;

  // Initialization of the algorithm
  for (const auto& [id, stop]: stops_) {
    min_arrival_time[id] = std::vector<StopInfo>(1, {INF, -1, -1});
  }
  min_arrival_time[query.source_id][0].min_arrival_time = timeToSeconds(query.departure_time);
  marked_stops.insert(query.source_id);

  int k = 1;
  while (true) {
    // 1st: set an upper bound
    for (const auto& [id, stop]: stops_){
      min_arrival_time[id][k].min_arrival_time = min_arrival_time[id][k-1].min_arrival_time;
      min_arrival_time[id][k].parent_trip_id = min_arrival_time[id][k-1].parent_trip_id;
      min_arrival_time[id][k].parent_stop_id = min_arrival_time[id][k-1].parent_stop_id;
    }

    // Accumulate routes serving marked stops from previous round
    std::queue<int> routes_ids_queue;

    // For each marked stop p
    auto marked_stop_id = marked_stops.begin();
    while (marked_stop_id != marked_stops.end()) {

      // For each route r serving p
      for (const auto &route_id: stops_[*marked_stop_id].routes_ids) {

        // TODO: does it need to be a queue?
        // TODO:
        // if (r, p') E Q for some stop p'
        //  if p comes before p' in r
        //    Substitute (r, p') by (r, p) in Q
        //  else
        //    Add(r, p) to Q

        routes_ids_queue.push(route_id);
        std::cout << "Added route  " << route_id << " that serves stop " << *marked_stop_id << std::endl;
      }
      // Unmark p
      marked_stop_id = marked_stops.erase(marked_stop_id); // Next valid stop
    }

    // 2nd: Traverse each route
    while (!routes_ids_queue.empty()) {
      const Route& route = routes_[routes_ids_queue.front()];
      routes_ids_queue.pop();

      std::cout << "Traversing route " << route.route_id << " that has " << route.stops.size() << " stops" << std::endl;

      // For each stop on this route, try to find the earliest trip (et) that can be taken
      for (size_t i = 0; i < route.stops.size(); ++i) {
        int stop_id = route.stops[i]->stop_id;
        int et_id = -1;

        // Find the earliest trip that can be caught at stop in round k
        for (StopTime* stop_time : stops_[stop_id].stop_times){
          if (timeToSeconds(stop_time->departure_time) >= min_arrival_time[stop_id][k-1].min_arrival_time){
            et_id = stop_time->trip_id;
            std::cout << "et_id = " << et_id << std::endl;
            break; // We can break because stop_times is ordered
          }
        }

        if (et_id == -1) continue; // No valid trip found for this stop

        Trip et = trips_[et_id];

        std::cout << "Successfully accessed et." << std::endl;

        // Traverse remaining stops on the route to update arrival times
        for (size_t j = i + 1; j < route.stops.size(); ++j) { // j is the next stop

          std::cout << "Stop " << route.stops[j]->stop_id << " has " << route.stops[j]->stop_times.size() << " stop_times." << std::endl;
          std::cout << "Trip " << et.trip_id << " has " << et.stop_times.size() << " stop_times."<< std::endl;

          // Calculate the arrival time (at the next stop or final?)
          int arrival_time = timeToSeconds(et.stop_times[j]->arrival_time);

          int next_stop_id = route.stops[j]->stop_id;

          std::cout << "Analyzing remaining stop " << next_stop_id << std::endl;

          // If arrival time can be improved
//        if (arrival_time < std::min(min_arrival_time[stop_id][k], min_arrival_time[query.target_id][k])) {
          if (arrival_time < min_arrival_time[next_stop_id][k].min_arrival_time) {
            min_arrival_time[next_stop_id][k].min_arrival_time = arrival_time;
            min_arrival_time[next_stop_id][k].parent_trip_id = et_id;
            min_arrival_time[next_stop_id][k].parent_stop_id = route.stops[i]->stop_id;

            marked_stops.insert(next_stop_id); // Mark this stop for the next round

            std::cout << "Marked stop " << next_stop_id << std::endl;
          }

          // Check if a faster trip can be caught at stop j
          if (j < route.stops.size() - 1) {
            StopTime* next_stop_time = stops_[next_stop_id].stop_times[j];
            if (min_arrival_time[next_stop_id][k - 1].min_arrival_time < timeToSeconds(next_stop_time->arrival_time)) { // arrival or departure?
              et_id = next_stop_time->trip_id;
            }
          }

        } // end remaining stops on route

      } // end each stop on route

    } // end each route

    // Look for foot-paths
    // For each marked stop p
    for (const auto& stop_id: marked_stops) {
      // For each foot-path (p, p')
      for (const auto& [dest_id, footpath]: stops_[stop_id].footpaths) {
        int new_arrival = min_arrival_time[stop_id][k].min_arrival_time + footpath.duration;
        if (new_arrival < min_arrival_time[dest_id][k].min_arrival_time) {
          min_arrival_time[dest_id][k].min_arrival_time = new_arrival;
          min_arrival_time[dest_id][k].parent_trip_id = -1;
          min_arrival_time[dest_id][k].parent_stop_id = stop_id;
          marked_stops.insert(dest_id);
        }
      }
    }

    // Stopping criterion
    // if no stops are marked, then stop
    if (marked_stops.empty()) {
      break;
    }

    k++;
  }
  std::cout << "Reconstructing journeys..." << std::endl;
  std::vector<std::vector<JourneyStep>> journeys = reconstructJourneys(query, k);
  return journeys;
}


std::vector<std::vector<JourneyStep>> Raptor::reconstructJourneys(const Query &query, int k) {
  std::vector<std::vector<JourneyStep>> journeys;

  // Start from the target stop and reconstruct the journey
  while (k >= 0) {
    std::vector<JourneyStep> journey;
    int current_stop_id = query.target_id;
    bool found_journey = false;

    std::cout << "current_stop_id: " << current_stop_id << std::endl;

    while (current_stop_id != -1) {
      const int parent_trip_id = min_arrival_time[current_stop_id][k].parent_trip_id;
      const int parent_stop_id = min_arrival_time[current_stop_id][k].parent_stop_id;

      std::cout << "parent_trip_id: " << parent_trip_id << std::endl;
      std::cout << "parent_stop_id: " << parent_stop_id << std::endl;

      if (parent_trip_id == -1) { // Footpath
        int footpath_duration = stops_[parent_stop_id].footpaths[current_stop_id].duration;
        int departure_time = min_arrival_time[parent_stop_id][k].min_arrival_time;
        int arrival_time = departure_time + footpath_duration;
        journey.push_back({-1, parent_stop_id, current_stop_id, departure_time, arrival_time});
        break;
      } else { // Trip
        // TODO: trip departure time
        journey.push_back({parent_trip_id, parent_stop_id, current_stop_id, 0, min_arrival_time[current_stop_id][k].min_arrival_time});
      }

      // Update to the previous stop boarded
      current_stop_id = parent_stop_id;

      // Make sure we only add valid journeys
      if (current_stop_id == query.source_id) {
        std::cout << "Found a journey!" << std::endl;
        found_journey = true;
        break;
      }
    }

    if (found_journey) {
      // Reverse the journey to obtain the correct sequence
      std::reverse(journey.begin(), journey.end());

      journeys.push_back(journey);
      std::cout << "Added a journey with k = " << k << std::endl;
    }

    // Decrease to the next journey with fewer transfers
    k--;
  }
  return journeys;
}

