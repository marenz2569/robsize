#pragma once

#include "robsize/CompiledTest.hpp"

#include <asmjit/asmjit.h>
#include <string>
#include <utility>
#include <vector>

namespace robsize {

/// The class that handles the creation of the test that accesses the cache twice and adds filler instructions
/// inbetween.
class CacheMissTest {
private:
  std::string Name;

public:
  /// Construct the test with a given name.
  explicit CacheMissTest(std::string Name)
      : Name(std::move(Name)){};
  virtual ~CacheMissTest() = default;

  /// Get the name of the test
  [[nodiscard]] auto name() const -> const auto& { return Name; }

  /// Create the test based on some given test parameters.
  /// \arg InstructionCount The number of filler instructions between the cache misses.
  /// \arg InnerLoopRepetitions The number of time the inner unrolled loop gets repeateadly executed.
  /// \arg UnrollCount The number of times the inner loop is unrolled
  auto compileTest(unsigned InstructionCount, unsigned InnerLoopRepetitions, unsigned UnrollCount)
      -> CompiledTest::UniquePtr;

protected:
  /// Virtual method to add the filler instructions between the two cache accesses that should miss.
  /// \arg Cb The Builder that is used to emit the assembler instructions
  /// \arg InstructionCount The number of filler instructions between the cache misses.
  /// \arg AvailableGpRegisters The general purpose registers which are available for the filler instructions to use.
  virtual void addFillerInstructions(asmjit::x86::Builder& Cb, unsigned InstructionCount,
                                     const std::vector<asmjit::x86::Gpq>& AvailableGpRegisters) = 0;
};

} // namespace robsize