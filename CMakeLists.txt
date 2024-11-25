cmake_minimum_required(VERSION 3.28)
project(raptor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include_directories(.)

add_subdirectory(Google_tests)

add_executable(raptor
        src/main.cpp
        src/Parser.cpp
        src/Parser.h
        src/Raptor.cpp
        src/Raptor.h
        src/Utils.h
        src/Utils.cpp
        src/Application.h
        src/Application.cpp
        src/NetworkObjects/DataStructures.h
        src/NetworkObjects/GTFSObjects/Route.h
        src/NetworkObjects/GTFSObjects/Route.cpp
        src/NetworkObjects/GTFSObjects/Trip.cpp
        src/NetworkObjects/GTFSObjects/Trip.h
        src/NetworkObjects/GTFSObjects/Stop.cpp
        src/NetworkObjects/GTFSObjects/Stop.h
        src/NetworkObjects/GTFSObjects/GTFSObject.cpp
        src/NetworkObjects/GTFSObjects/GTFSObject.h
        src/NetworkObjects/GTFSObjects/Agency.cpp
        src/NetworkObjects/GTFSObjects/Agency.h
        src/NetworkObjects/GTFSObjects/Calendar.cpp
        src/NetworkObjects/GTFSObjects/Calendar.h
        src/NetworkObjects/GTFSObjects/StopTime.cpp
        src/NetworkObjects/GTFSObjects/StopTime.h
        src/Constants.h)
