# RAPTOR - Round-Based Public Transit Routing Algorithm
This project implements the Round-Based Public Transit Routing (RAPTOR) algorithm, 
enabling efficient public transport route searches based on GTFS data.

### Setting up the Project Environment
1. **Clone the Repository**: To clone the repository, use the following command:
    ```bash 
    git clone --recurse-submodules git@github.com:mariaarabelo/RAPTOR.git
    ```

The ``--recurse-submodules flag`` ensures that the submodules (Google Test) are initialized properly. 
    
If you already cloned the repository without this flag, you can initialize submodules later using:
    
```bash 
git submodule update --init --recursive
```
    
2. **Install Dependencies**:
 - **g++ (C++20)**
 - **CMake 3.22 or later**

3. **Build the Project**: Run the following commands in a UNIX-like terminal:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

### Running the Program
You can run the program by executing the following commands in the terminal:

```bash
Copy code
./RAPTOR ../datasets/Porto/metro/GTFS/ ../datasets/Porto/stcp/GTFS/
```

You can specify the path to the GTFS directories directly in the command line.

If no path is provided, the program will prompt you to enter the directory path.

### Project Structure
- **src/**: Contains the main RAPTOR algorithm implementation and supporting code.
- **datasets/**: Directory for storing GTFS data.
- **tests/**: Directory for test files and Google Test submodule.
- **docs/**: Contains the Doxygen configuration file for generating documentation.

## Using the CMake Configuration
The CMake configuration is set up to create both the main executable (RAPTOR) and a library (raptor_lib). The tests are handled separately with Google Test included as a submodule.

Here's the breakdown of the CMake files:

CMakeLists.txt (for the main project):

Specifies the C++20 standard.
Adds the raptor_lib library with all the source files required for the project.
Links the raptor_lib library to the main executable (RAPTOR).
CMakeLists.txt (for tests):

Specifies the C++17 standard for tests.
Includes the Google Test submodule and links it with the raptor_lib library.
Adds the test executable (Google_Tests_run).
References
Delling, Daniel, Thomas Pajor, Renato F. Werneck, “Round-based Public Transit Routing.” Microsoft Research (2012). Link to paper
GTFS Schedule Documentation (2024) Link to GTFS documentation
Raptor, another journey planning algorithm (2018) Link to Raptor algorithm explanation
Additional Notes
Google Test Submodule: The tests/lib folder is a Git submodule that includes Google Test. Ensure it is initialized properly using the git submodule update --init command.
CMake Configuration: The project uses CMake for building. Be sure to run cmake .. inside the build directory to configure the project and generate the necessary build files.
This updated README provides a complete guide to setting up the project, compiling it, and running it on your local machine with the appropriate environ







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
- GTFS Schedule Documentation (2024) https://gtfs.org/documentation/schedule/reference/
- Raptor, another journey planning algorithm (2018) https://ljn.io/posts/raptor-journey-planning-algorithm