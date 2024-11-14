#include <iostream>
#include "Application.h"

int main(int argc, char *argv[]) {

  std::string inputDirectory;

  if (argc >= 2) {
    inputDirectory = argv[1];
  } else {
    std::cout << "GTFS Input Directory: ";
    std::getline(std::cin, inputDirectory);
  }

  Application application(inputDirectory);
  application.run();

  // query 5777 5776 22:00:00
  // trip stop  dep_time  -> stop   arr_time
  // DU301 5777 22:01:00  5776      21:59:00

  // DU404,22:05:00,22:05:00,5777,16,,,,
  // DU404,22:07:00,22:07:00,5776,17,,,,



  return 0;
}
