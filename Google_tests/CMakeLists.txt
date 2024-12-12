# 'Google_tests' is the subproject name
project(Google_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Include the Google Test submodule
add_subdirectory(lib)
include_directories(${CMAKE_SOURCE_DIR}/Google_tests/lib/googletest/include)

# Add an executable target for tests
add_executable(Google_Tests_run
        test_findJourneys.cpp # Add test files
)

# Link Google Test libraries and project files
target_link_libraries(Google_Tests_run gtest gtest_main raptor_lib)

target_compile_definitions(Google_Tests_run PRIVATE DATASET_PATH="${CMAKE_SOURCE_DIR}/datasets")