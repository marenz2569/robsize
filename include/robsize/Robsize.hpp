#pragma once

#include "robsize/CacheMissTest.hpp"
#include "robsize/Config.hpp"
#include "robsize/SingleInstructionTest.hpp"

#include <map>
#include <memory>
#include <vector>

namespace robsize {

// 1 MiB
constexpr const std::size_t AddressBufferSize = 1048576;

struct InstructionCountResult {
  uint64_t MinCycles;
  uint64_t AverageCycles;
  uint64_t MaxCycles;
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
  /// \arg Unroll The number of unrolls of the experiment loop
  /// \arg InnerIterations The number of iteration of the experiment loop
  /// \arg OuterIterations The number of iterations used to average the result
  /// \arg TestId The id of the test that should be executed
  [[nodiscard]] auto runTest(unsigned Start, unsigned Stop, unsigned Unroll, unsigned InnerIterations,
                             unsigned OuterIterations, unsigned TestId) -> RobsizeResult;

  /// Const getter for the tests
  [[nodiscard]] auto avaialableTests() const -> const std::vector<std::shared_ptr<CacheMissTest>>& {
    return AvailableTests;
  }

private:
  /// The available tests that can be used by robsize. The test index maps to the same element in this vector.
  std::vector<std::shared_ptr<CacheMissTest>> AvailableTests = {
      std::make_shared<SingleInstructionTest<InstructionType::kNopInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kTwoByteNopInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kAddInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kMovInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kCmpInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXorInstruction>>(),
  };
};

} // namespace robsize