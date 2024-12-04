#pragma once

#include "robsize/CacheMissTest.hpp"
#include "robsize/Config.hpp"
#include "robsize/SingleInstructionTest.hpp"

#include <map>
#include <memory>
#include <vector>

namespace robsize {

struct InstructionCountResult {
  uint64_t Cycles;
};

struct RobsizeResult {
  unsigned TestId;
  std::map<unsigned, InstructionCountResult> InstructionCountResults;
};

struct RobsizeResults {
  RobsizeResults() = default;

  explicit RobsizeResults(const RobsizeResult& Result) {
    RobsizeResults Results{};
    TestResults.emplace_back(Result);
  }

  std::vector<RobsizeResult> TestResults;
};

class RobsizeTest {
public:
  RobsizeTest() = default;

  /// Run the robsize tests
  /// \arg Cgf The config for the robsize run
  [[nodiscard]] auto runTests(const robsize::Config& Cfg) -> RobsizeResults;

  /// Run a specific robsize test
  /// \arg Start The minimum number of filler instructions that will be used to test the robsize.
  /// \arg Stop The maximum number of filler instructions that will be used to test the robsize.
  /// \arg TestId The id of the test that should be executed
  [[nodiscard]] auto runTest(unsigned Start, unsigned Stop, unsigned TestId) -> RobsizeResult;

  /// Const getter for the tests
  [[nodiscard]] auto avaialableTests() const -> const std::vector<std::shared_ptr<CacheMissTest>>& {
    return AvailableTests;
  }

private:
  /// The available tests that can be used by robsize. The test index maps to the same element in this vector.
  std::vector<std::shared_ptr<CacheMissTest>> AvailableTests = {
      std::make_shared<SingleInstructionTest<InstructionType::kAddInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kMovInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kCmpInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXorInstruction>>(),
  };
};

} // namespace robsize