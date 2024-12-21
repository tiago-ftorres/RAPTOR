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

3. **Build the Program**:
Run the following commands in a UNIX-like terminal:
```bash
mkdir build
cd build
cmake ..
make
```

### Running the Program

You can run the program by executing one of the following commands in the terminal:

```bash
./RAPTOR ../datasets/Porto/metro/GTFS/ ../datasets/Porto/stcp/GTFS/
./RAPTOR ../datasets/Porto/metro/GTFS/
./RAPTOR ../datasets/Porto/stcp/GTFS/
```

You can specify the path to the GTFS directories directly in the command line.

If no path is provided, the program will prompt you to enter the directory path.

### Running the Tests
You can run the tests by using the following command:

```bash
./tests/TESTS
```

From the tests folder, to automatically run all tests, you can run the command:

```bash 
ctest
```

### Project Structure
- **src/**: Contains the main RAPTOR algorithm implementation and supporting code.
- **datasets/**: Directory for storing GTFS data.
- **tests/**: Directory for test files and Google Test submodule.
- **docs/**: Contains the Doxygen configuration file for generating documentation.

This repository is developed as part of my internship at OPT (Optimizações e Planeamento de Transporte). 

### References
- Delling, Daniel, Thomas Pajor, Renato F. Werneck, “Round-based Public Transit Routing.” Microsoft Research (2012). https://www.microsoft.com/en-us/research/wp-content/uploads/2012/01/raptor_alenex.pdf
- GTFS Schedule Documentation (2024) https://gtfs.org/documentation/schedule/reference/
- Raptor, another journey planning algorithm (2018) https://ljn.io/posts/raptor-journey-planning-algorithm
