# RAPTOR - Round-Based Public Transit Routing Algorithm

To compile the project, use the following command in WSL:

<code> 
g++ -std=c++20 -o raptor main.cpp Parser.cpp Raptor.cpp Utils.cpp Application.cpp \ 
NetworkObjects/GTFSObjects/Route.cpp NetworkObjects/GTFSObjects/Trip.cpp \
NetworkObjects/GTFSObjects/Stop.cpp NetworkObjects/GTFSObjects/GTFSObject.cpp \
NetworkObjects/GTFSObjects/Agency.cpp NetworkObjects/GTFSObjects/Calendar.cpp \
NetworkObjects/GTFSObjects/StopTime.cpp 
</code>

You can run the program by executing one of the following command in the terminal:

``` ./RAPTOR ./dataset/Porto/metro/GTFS/```

``` ./RAPTOR ./dataset/Porto/sctp/GTFS/```

 - You can specify the path to the GTFS directory directly in the command line. 
 - If no path is provided, the program will prompt you to enter the directory path.


This repository is developed as part of my internship at OPT (Optimizações e Planeamento de Transporte). 

### References
- Delling, Daniel, Thomas Pajor, Renato F. Werneck, “Round-based Public Transit Routing.” Microsoft Research (2012). https://www.microsoft.com/en-us/research/wp-content/uploads/2012/01/raptor_alenex.pdf
