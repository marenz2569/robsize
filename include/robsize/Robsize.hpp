#pragma once

#include "robsize/CacheMissTest.hpp"
#include "robsize/Config.hpp"
#include "robsize/RobsizeResults.hpp"
#include "robsize/SingleInstructionTest.hpp"

namespace robsize {

// 1 MiB
constexpr const std::size_t AddressBufferSize = 1048576;

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
                             unsigned OuterIterations, unsigned TestId) -> std::vector<RobsizeResult>;

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
      std::make_shared<SingleInstructionTest<InstructionType::kAddInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kMovInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kCmpInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXorInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmAddInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmMovInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmCmpInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmXorInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmAddInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmMovInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmCmpInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmXorInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmAddInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmMovInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmCmpInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmXorInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmAddInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmMovInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmCmpInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmXorInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmAddInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmMovInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmCmpInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmXorInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmAddInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmMovInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmCmpInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmXorInstructionAlternating>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kLoadInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmLoadInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmLoadInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmLoadInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kStoreInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kXmmStoreInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kYmmStoreInstruction>>(),
      std::make_shared<SingleInstructionTest<InstructionType::kZmmStoreInstruction>>(),
  };
};

} // namespace robsize