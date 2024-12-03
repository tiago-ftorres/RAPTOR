#include <iostream>
#include "Application.h"

int main(int argc, char *argv[]) {

  std::vector<std::string> inputDirectories;

  if (argc >= 2) {
    for (int i = 1; i < argc; ++i) {
      inputDirectories.emplace_back(argv[i]);
    }
  } else {
    std::string input;
    std::cout << "Enter GTFS Input Directories (one per line). Type 'done' to finish, or enter a blank line: " << std::endl;

    while (true) {
      std::getline(std::cin, input);

      if (input == "done" || input.empty()) break;
      inputDirectories.push_back(input);
    }

  }

  Application application(inputDirectories);
  application.run();

  return 0;
}
