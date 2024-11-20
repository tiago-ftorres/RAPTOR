cmake_minimum_required(VERSION 3.28)
project(raptor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include_directories(.)

add_executable(raptor
        NetworkObjects/DataStructures.h
        main.cpp
        Parser.cpp
        Parser.h
        Raptor.cpp
        Raptor.h
        Utils.h
        Utils.cpp
        Application.h
        Application.cpp
        NetworkObjects/GTFSObjects/Route.h
        NetworkObjects/GTFSObjects/Route.cpp
        NetworkObjects/GTFSObjects/Trip.cpp
        NetworkObjects/GTFSObjects/Trip.h
        NetworkObjects/GTFSObjects/Stop.cpp
        NetworkObjects/GTFSObjects/Stop.h
        NetworkObjects/GTFSObjects/GTFSObject.cpp
        NetworkObjects/GTFSObjects/GTFSObject.h
        NetworkObjects/GTFSObjects/Agency.cpp
        NetworkObjects/GTFSObjects/Agency.h
        NetworkObjects/GTFSObjects/Calendar.cpp
        NetworkObjects/GTFSObjects/Calendar.h
        NetworkObjects/GTFSObjects/StopTime.cpp
        NetworkObjects/GTFSObjects/StopTime.h)
