# RAPTOR - Round-Based Public Transit Routing Algorithm

To compile the project, use the following command:

``` g++ -o RAPTOR main.cpp Application.cpp Raptor.cpp Parse.cpp Utils.cpp -std=c++20 ```

You can run the program by executing the following command in the terminal:

 ``` ./RAPTOR ./dataset/Porto/small/GTFS/```

 - You can specify the path to the GTFS directory directly in the command line. 
 - If no path is provided, the program will prompt you to enter the directory path.


This repository is developed as part of my internship at OPT (Optimizações e Planeamento de Transporte). 

### References
- Delling, Daniel, Thomas Pajor, Renato F. Werneck, “Round-based Public Transit Routing.” Microsoft Research (2012). https://www.microsoft.com/en-us/research/wp-content/uploads/2012/01/raptor_alenex.pdf
