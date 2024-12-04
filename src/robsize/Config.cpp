#include "robsize/Config.hpp"

#include <cstdlib>
#include <cxxopts.hpp>
#include <iostream>
#include <stdexcept>

namespace robsize {

Config::Config(int Argc, const char** Argv)
    : Argc(Argc)
    , Argv(Argv) {
  const auto* ExecutableName = *Argv;

  cxxopts::Options Parser(ExecutableName);

  // clang-format off
  Parser.add_options()
    ("l,list", "List the available tests")
    ("start", "The minimal number of filler instructions", cxxopts::value<unsigned>()->default_value("16"))
    ("stop", "The maximum number of filler instructions", cxxopts::value<unsigned>()->default_value("1024"))
    ("testid", "The number of the test to execute. If this is not given all are executed", cxxopts::value<unsigned>())
  ;
  // clang-format on

  try {
    auto Options = Parser.parse(Argc, Argv);

    PrintTests = Options.contains("list");

    if (Options.contains("testid")) {
      TestId = Options["testid"].as<unsigned>();
    }

    Start = Options["start"].as<unsigned>();
    Stop = Options["stop"].as<unsigned>();

    if (Start > Stop) {
      throw std::runtime_error("The value supplied for --start is bigger than the value for --stop.");
    }
  } catch (std::exception& E) {
    std::cerr << Parser.help() << "\n";
    std::cerr << "\n";
    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    exit(EXIT_SUCCESS);
  }
}

} // namespace robsize