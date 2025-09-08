#include "robsize/Config.hpp"

#include <cstddef>
#include <cstdlib>
#include <cxxopts.hpp>
#include <exception>
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
    ("unroll", "The number of unrolls of the experiment loop", cxxopts::value<unsigned>()->default_value("16"))
    ("inner-iterations", "The number of iteration of the experiment loop", cxxopts::value<unsigned>()->default_value("2048"))
    ("outer-iterations", "The number of iterations used to average the result", cxxopts::value<unsigned>()->default_value("16"))
    ("testid", "The number of the test to execute. If this is not given all are executed", cxxopts::value<unsigned>())
    ("outfile", "The path where the results should be saved to.", cxxopts::value<std::string>()->default_value("outfile.csv"))
    ("cpu-index", "The os index to which the experiment thread should be bound.", cxxopts::value<std::size_t>()->default_value("0"))
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

    UnrollCount = Options["unroll"].as<unsigned>();
    InnerIterations = Options["inner-iterations"].as<unsigned>();
    OuterIterations = Options["outer-iterations"].as<unsigned>();

    CpuIndex = Options["cpu-index"].as<std::size_t>();

    OutfilePath = Options["outfile"].as<std::string>();

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