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

  Config() = delete;

  /// Parser the config from the command line argumens.
  Config(int Argc, const char** Argv);
};

} // namespace robsize