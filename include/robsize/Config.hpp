#pragma once

#include <optional>

namespace robsize {

/// This struct contains the parsed config from the command line for robsized.
struct Config {
  /// The argument vector from the command line.
  const char** Argv;
  /// The argument count from the command line.
  int Argc;

  /// The number of the test that should be executed. If this is nullopt, then all tests shall be executed.
  std::optional<unsigned> TestId;

  /// The minimum number of filler instructions that will be used to test the robsize.
  unsigned Start;
  /// The maximum number of filler instructions that will be used to test the robsize.
  unsigned Stop;

  /// The number of unrolls of the experiment loop
  unsigned UnrollCount;

  /// The number of iteration of the experiment loop
  unsigned InnerIterations;

  /// The number of iterations used to average the result
  unsigned OuterIterations;

  /// Print the available tests with their names
  bool PrintTests;

  Config() = delete;

  /// Parser the config from the command line argumens.
  Config(int Argc, const char** Argv);
};

} // namespace robsize