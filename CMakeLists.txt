cmake_minimum_required(VERSION 3.28)
project(raptor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include_directories(.)

add_subdirectory(Google_tests)

add_library(raptor_lib
        src/Parser.cpp
        src/Raptor.cpp
        src/Utils.cpp
        src/Application.cpp
        src/DateTime.h
        src/NetworkObjects/DataStructures.h
        src/NetworkObjects/GTFSObjects/Route.cpp
        src/NetworkObjects/GTFSObjects/Trip.cpp
        src/NetworkObjects/GTFSObjects/Stop.cpp
        src/NetworkObjects/GTFSObjects/GTFSObject.cpp
        src/NetworkObjects/GTFSObjects/Agency.cpp
        src/NetworkObjects/GTFSObjects/Calendar.cpp
        src/NetworkObjects/GTFSObjects/StopTime.cpp
)

target_include_directories(raptor_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(raptor src/main.cpp)
target_link_libraries(raptor raptor_lib)
