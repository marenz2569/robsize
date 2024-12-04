#include "robsize/Config.hpp"
#include "robsize/Robsize.hpp"

#include <cstdlib>
#include <iostream>

auto main(int Argc, const char** Argv) -> int {
  std::cout << "robsize - Microarchitectural benchmark to measure the reorder buffer size based on the methodoly "
               "described by Henry Wong, Version "
            << _ROBSIZE_VERSION_STRING << "\n"
            << "Copyright (C) " << _ROBSIZE_BUILD_YEAR << " Markus Schmidl"
            << "\n";

  try {
    robsize::Config Cfg{Argc, Argv};

    robsize::RobsizeTest Robsize;

    if (Cfg.PrintTests) {
      std::cout << "Index TestName\n";
      auto I = 0U;
      for (const auto& Test : Robsize.avaialableTests()) {
        std::cout << I++ << " " << Test->name() << "\n";
      }
      return EXIT_SUCCESS;
    }

    auto Results = Robsize.runTests(Cfg);

    for (const auto& Tests : Results.TestResults) {
      std::cout << "Results for test: " << Tests.TestId << "\n";
      for (const auto& [Key, Value] : Tests.InstructionCountResults) {
        std::cout << Key << "," << Value.Cycles << "\n";
      }
    }
  } catch (std::exception const& E) {
    std::cerr << E.what();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}