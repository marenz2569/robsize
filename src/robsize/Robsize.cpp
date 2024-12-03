#include "robsize/Robsize.hpp"

namespace robsize {

auto RobsizeTest::runTests(const robsize::Config& Cfg) -> RobsizeResults {
  // Run a single selected test
  if (Cfg.TestId) {
    auto TestResult = runTest(Cfg.Start, Cfg.Stop, *Cfg.TestId);
    return RobsizeResults(TestResult);
  }

  // TODO: Run all tests
  return RobsizeResults{};
}

auto RobsizeTest::runTest(unsigned Start, unsigned Stop, unsigned TestId) -> RobsizeResult {
  // Run the test for each number of filler instructions
  for (decltype(Start) InstructionCount = Start; InstructionCount <= Stop; InstructionCount++) {
  }

  return RobsizeResult{};
}

} // namespace robsize
