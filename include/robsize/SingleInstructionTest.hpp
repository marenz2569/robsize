#pragma once

#include "robsize/CacheMissTest.hpp"

namespace robsize {

// NOLINTNEXTLINE(performance-enum-size)
enum class InstructionType {
  kNopInstruction,
  kTwoByteNopInstruction,
  kAddInstruction,
  kMovInstruction,
  kCmpInstruction,
  kXorInstruction,
  kAddInstructionAlternating,
  kMovInstructionAlternating,
  kCmpInstructionAlternating,
  kXorInstructionAlternating,
};

constexpr auto getName(enum InstructionType Type) -> const char* {
  switch (Type) {
  case InstructionType::kNopInstruction:
    return "Single byte Nop instruction";
  case InstructionType::kTwoByteNopInstruction:
    return "Two byte Nop instruction";
  case InstructionType::kAddInstruction:
    return "Add instruction without alternating registers";
  case InstructionType::kMovInstruction:
    return "Mov instruction without alternating registers";
  case InstructionType::kCmpInstruction:
    return "Cmp instruction without alternating registers";
  case InstructionType::kXorInstruction:
    return "Xor instruction without alternating registers";
  case InstructionType::kAddInstructionAlternating:
    return "Add instruction with alternating registers";
  case InstructionType::kMovInstructionAlternating:
    return "Mov instruction with alternating registers";
  case InstructionType::kCmpInstructionAlternating:
    return "Cmp instruction with alternating registers";
  case InstructionType::kXorInstructionAlternating:
    return "Xor instruction with alternating registers";
  default:
    return "unknown";
  }
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
      const auto& NextGpRegister = AvailableGpRegisters.at((I + 1) % AvailableGpRegisters.size());
      switch (Type) {
      case InstructionType::kNopInstruction:
        Cb.nop();
        break;
      case InstructionType::kTwoByteNopInstruction:
        Cb.xchg(CurrentGpRegister.r8Lo(), CurrentGpRegister.r8Lo());
        break;
      case InstructionType::kAddInstruction:
        Cb.add(CurrentGpRegister, CurrentGpRegister);
        break;
      case InstructionType::kMovInstruction:
        Cb.mov(CurrentGpRegister, CurrentGpRegister);
        break;
      case InstructionType::kCmpInstruction:
        Cb.cmp(CurrentGpRegister, CurrentGpRegister);
        break;
      case InstructionType::kXorInstruction:
        Cb.xor_(CurrentGpRegister, CurrentGpRegister);
        break;
      case InstructionType::kAddInstructionAlternating:
        Cb.add(CurrentGpRegister, NextGpRegister);
        break;
      case InstructionType::kMovInstructionAlternating:
        Cb.mov(CurrentGpRegister, NextGpRegister);
        break;
      case InstructionType::kCmpInstructionAlternating:
        Cb.cmp(CurrentGpRegister, NextGpRegister);
        break;
      case InstructionType::kXorInstructionAlternating:
        Cb.xor_(CurrentGpRegister, NextGpRegister);
        break;
      }
    }
  }
};

} // namespace robsize
