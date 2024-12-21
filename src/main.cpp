/**
 * @file main.cpp
 * @brief Entry point for the RAPTOR application.
 *
 * This file initializes the application, parses input directories, and starts
 * the main event loop for processing user queries.
 */

#include <iostream>
#include "Application.h"

/**
 * @brief Main function for the RAPTOR application.
 *
 * This function parses command-line arguments or prompts the user for GTFS input directories,
 * initializes the application, and starts the interactive event loop.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit status of the application.
 */
int main(int argc, char *argv[]) {
  std::vector<std::string> inputDirectories;

  // Parse command-line arguments for input directories
  if (argc >= 2) {
    for (int i = 1; i < argc; ++i)
      inputDirectories.emplace_back(argv[i]);

  } else {
    // Prompt user for input directories
    std::string input;
    std::cout << "Enter GTFS Input Directories (one per line). Type 'done' to finish, or enter a blank line: " << std::endl;

    while (true) {
      std::getline(std::cin, input);
      if (input == "done" || input.empty()) break;
      inputDirectories.push_back(input);
    }
  }

  // Initialize and run the application
  Application application(inputDirectories);
  application.run();

  return 0;
}
