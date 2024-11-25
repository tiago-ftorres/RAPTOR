# 'Google_tests' is the subproject name
project(Google_tests)

# Include the Google Test submodule
add_subdirectory(lib)
include_directories(${CMAKE_SOURCE_DIR}/Google_tests/lib/googletest/include)

# Add an executable target for tests
add_executable(Google_Tests_run
        test_findJourneys.cpp # Add test files
)

# Link Google Test libraries
target_link_libraries(Google_Tests_run gtest gtest_main)
