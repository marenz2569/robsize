#include "robsize/Config.hpp"

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
  } catch (std::exception const& E) {
    std::cerr << E.what();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}