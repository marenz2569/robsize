#pragma once

#include "robsize/CacheMissTest.hpp"

namespace robsize {

// NOLINTNEXTLINE(performance-enum-size)
enum class InstructionType {
  kAddInstruction,
  kMovInstruction,
  kCmpInstruction,
  kXorInstruction,
};

constexpr auto getName(enum InstructionType Type) -> const char* {
  switch (Type) {
  case InstructionType::kAddInstruction:
    return "Add instruction without alternating registers";
  case InstructionType::kMovInstruction:
    return "Mov instruction without alternating registers";
  case InstructionType::kCmpInstruction:
    return "Cmp instruction without alternating registers";
  case InstructionType::kXorInstruction:
    return "Xor instruction without alternating registers";
  }
  return "unknown";
}

template <InstructionType Type> class SingleInstructionTest : public CacheMissTest {
public:
  SingleInstructionTest()
      : CacheMissTest(getName(Type)) {}

private:
  /// Method to add the filler instructions between the two cache accesses that should miss. The specific instruction is
  /// defined by the template argument to the class.
  /// \arg Cb The Builder that is used to emit the assembler instructions
  /// \arg InstructionCount The number of filler instructions between the cache misses.
  /// \arg AvailableGpRegisters The general purpose registers which are available for the filler instructions to use.
  void addFillerInstructions(asmjit::x86::Builder& Cb, unsigned InstructionCount,
                             const std::vector<asmjit::x86::Gpq>& AvailableGpRegisters) final {
    for (auto I = 0U; I < InstructionCount; I++) {
      // Iterate over the available registers and emit the specified instruction acording to specified instruction type.
      const auto& CurrentGpRegister = AvailableGpRegisters.at(I % AvailableGpRegisters.size());
      switch (Type) {
      case InstructionType::kAddInstruction:
        Cb.add(CurrentGpRegister, CurrentGpRegister);
      case InstructionType::kMovInstruction:
        Cb.mov(CurrentGpRegister, CurrentGpRegister);
      case InstructionType::kCmpInstruction:
        Cb.cmp(CurrentGpRegister, CurrentGpRegister);
      case InstructionType::kXorInstruction:
        Cb.xor_(CurrentGpRegister, CurrentGpRegister);
      }
    }
  }
};

} // namespace robsize
