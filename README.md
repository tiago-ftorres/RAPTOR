# RAPTOR - Round-Based Public Transit Routing Algorithm
This project implements the Round-Based Public Transit Routing (RAPTOR) algorithm, 
enabling efficient public transport route searches based on GTFS data.

### Prerequisites
- C++20 or later

To compile the project, use the following command in WSL:

<code> 
g++ -std=c++20 -o RAPTOR \
  src/main.cpp src/Parser.cpp src/Raptor.cpp src/Utils.cpp src/Application.cpp \
  src/NetworkObjects/GTFSObjects/Route.cpp src/NetworkObjects/GTFSObjects/Trip.cpp \
  src/NetworkObjects/GTFSObjects/Stop.cpp src/NetworkObjects/GTFSObjects/GTFSObject.cpp \
  src/NetworkObjects/GTFSObjects/Agency.cpp src/NetworkObjects/GTFSObjects/Calendar.cpp \
  src/NetworkObjects/GTFSObjects/StopTime.cpp
</code>

You can run the program by executing one of the following command in the terminal:

``` ./RAPTOR ./datasets/Porto/metro/GTFS/```

``` ./RAPTOR ./datasets/Porto/sctp/GTFS/```

 - You can specify the path to the GTFS directory directly in the command line. 
 - If no path is provided, the program will prompt you to enter the directory path.

### Project Structure
- **src/**: Contains the main RAPTOR algorithm implementation and supporting code.
- **datasets/**: Directory for storing GTFS data.

This repository is developed as part of my internship at OPT (Optimizações e Planeamento de Transporte). 

### References
- Delling, Daniel, Thomas Pajor, Renato F. Werneck, “Round-based Public Transit Routing.” Microsoft Research (2012). https://www.microsoft.com/en-us/research/wp-content/uploads/2012/01/raptor_alenex.pdf
