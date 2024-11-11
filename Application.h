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
  explicit Application(Raptor& raptor) : raptor(raptor) {}

  void run();

private:
  void handleQuery(const std::string& command);
  static void showCommands();

  Raptor& raptor;
};

#endif //RAPTOR_APPLICATION_H
