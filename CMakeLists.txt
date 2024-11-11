cmake_minimum_required(VERSION 3.28)
project(raptor)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(raptor
        DataStructures.h
        main.cpp
        Parse.cpp
        Parse.h
        Raptor.cpp
        Utils.h
        Utils.cpp
        Raptor.h
        Application.h
        Application.cpp)
