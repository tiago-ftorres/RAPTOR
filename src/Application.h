//
// Created by maria on 11/11/2024.
//

#ifndef RAPTOR_APPLICATION_H
#define RAPTOR_APPLICATION_H

#include "Raptor.h"
#include <iostream>
#include <iomanip>

class Application {
public:
  explicit Application(std::vector<std::string> inputDirectories);

  void run();

private:
  std::vector<std::string> inputDirectories;
  std::optional<Raptor> raptor_;

  void handleQuery();

  static void showCommands();

  std::string getSource();

  std::string getTarget();

  static Date getDate();

  static int getYear();

  static int getMonth();

  static int getDay(int year, int month);

  static Time getDepartureTime();

  static int getHours();

  static int getMinutes();
};

#endif //RAPTOR_APPLICATION_H
