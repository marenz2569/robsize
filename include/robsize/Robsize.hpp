#pragma once

#include "robsize/Config.hpp"

#include <vector>

namespace robsize {

struct RobsizeResult {
  unsigned TestId;
};

struct RobsizeResults {
  RobsizeResults() = default;

  explicit RobsizeResults(const RobsizeResult& Result) {
    RobsizeResults Results{};
    TestResults.emplace_back(Result);
  }

  std::vector<RobsizeResult> TestResults;
};

struct RobsizeTest {
public:
  /// Run the robsize tests
  /// \arg Cgf The config for the robsize run
  static auto runTests(const robsize::Config& Cfg) -> RobsizeResults;

  /// Run a specific robsize test
  /// \arg Start The minimum number of filler instructions that will be used to test the robsize.
  /// \arg Stop The maximum number of filler instructions that will be used to test the robsize.
  /// \arg TestId The id of the test that should be executed
  static auto runTest(unsigned Start, unsigned Stop, unsigned TestId) -> RobsizeResult;
};

} // namespace robsize