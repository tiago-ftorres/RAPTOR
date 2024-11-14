cmake_minimum_required(VERSION 3.28)
project(raptor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include_directories(.)

add_executable(raptor
        DataStructures.h
        main.cpp
        Parser.cpp
        Parser.h
        Raptor.cpp
        Raptor.h
        Utils.h
        Utils.cpp
        Application.h
        Application.cpp
        Parser.cpp
        gtfsClasses/Route.h)
