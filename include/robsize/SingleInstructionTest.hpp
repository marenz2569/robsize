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
  kXmmAddInstruction,
  kXmmMovInstruction,
  kXmmCmpInstruction,
  kXmmXorInstruction,
  kXmmAddInstructionAlternating,
  kXmmMovInstructionAlternating,
  kXmmCmpInstructionAlternating,
  kXmmXorInstructionAlternating,
  kYmmAddInstruction,
  kYmmMovInstruction,
  kYmmCmpInstruction,
  kYmmXorInstruction,
  kYmmAddInstructionAlternating,
  kYmmMovInstructionAlternating,
  kYmmCmpInstructionAlternating,
  kYmmXorInstructionAlternating,
  kZmmAddInstruction,
  kZmmMovInstruction,
  kZmmCmpInstruction,
  kZmmXorInstruction,
  kZmmAddInstructionAlternating,
  kZmmMovInstructionAlternating,
  kZmmCmpInstructionAlternating,
  kZmmXorInstructionAlternating,
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
  case InstructionType::kXmmAddInstruction:
    return "Xmm add instruction without alternating registers";
  case InstructionType::kXmmMovInstruction:
    return "Xmm mov instruction without alternating registers";
  case InstructionType::kXmmCmpInstruction:
    return "Xmm cmp instruction without alternating registers";
  case InstructionType::kXmmXorInstruction:
    return "Xmm xor instruction without alternating registers";
  case InstructionType::kXmmAddInstructionAlternating:
    return "Xmm add instruction with alternating registers";
  case InstructionType::kXmmMovInstructionAlternating:
    return "Xmm mov instruction with alternating registers";
  case InstructionType::kXmmCmpInstructionAlternating:
    return "Xmm cmp instruction with alternating registers";
  case InstructionType::kXmmXorInstructionAlternating:
    return "Xmm xor instruction with alternating registers";
  case InstructionType::kYmmAddInstruction:
    return "Ymm add instruction without alternating registers";
  case InstructionType::kYmmMovInstruction:
    return "Ymm mov instruction without alternating registers";
  case InstructionType::kYmmCmpInstruction:
    return "Ymm cmp instruction without alternating registers";
  case InstructionType::kYmmXorInstruction:
    return "Ymm xor instruction without alternating registers";
  case InstructionType::kYmmAddInstructionAlternating:
    return "Ymm add instruction with alternating registers";
  case InstructionType::kYmmMovInstructionAlternating:
    return "Ymm mov instruction with alternating registers";
  case InstructionType::kYmmCmpInstructionAlternating:
    return "Ymm cmp instruction with alternating registers";
  case InstructionType::kYmmXorInstructionAlternating:
    return "Ymm xor instruction with alternating registers";
  case InstructionType::kZmmAddInstruction:
    return "Zmm add instruction without alternating registers";
  case InstructionType::kZmmMovInstruction:
    return "Zmm mov instruction without alternating registers";
  case InstructionType::kZmmCmpInstruction:
    return "Zmm cmp instruction without alternating registers";
  case InstructionType::kZmmXorInstruction:
    return "Zmm xor instruction without alternating registers";
  case InstructionType::kZmmAddInstructionAlternating:
    return "Zmm add instruction with alternating registers";
  case InstructionType::kZmmMovInstructionAlternating:
    return "Zmm mov instruction with alternating registers";
  case InstructionType::kZmmCmpInstructionAlternating:
    return "Zmm cmp instruction with alternating registers";
  case InstructionType::kZmmXorInstructionAlternating:
    return "Zmm xor instruction with alternating registers";
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

      const auto& CurrentXmmRegister = asmjit::x86::Xmm(I % 16);
      const auto& NextXmmRegister = asmjit::x86::Xmm((I + 1) % 16);
      const auto& CurrentYmmRegister = asmjit::x86::Ymm(I % 16);
      const auto& NextYmmRegister = asmjit::x86::Ymm((I + 1) % 16);
      const auto& CurrentZmmRegister = asmjit::x86::Zmm(I % 16);
      const auto& NextZmmRegister = asmjit::x86::Zmm((I + 1) % 16);

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
      case InstructionType::kXmmAddInstruction:
        Cb.addpd(CurrentXmmRegister, CurrentXmmRegister);
        break;
      case InstructionType::kXmmMovInstruction:
        Cb.movdqa(CurrentXmmRegister, CurrentXmmRegister);
        break;
      case InstructionType::kXmmCmpInstruction:
        Cb.cmppd(CurrentXmmRegister, CurrentXmmRegister, asmjit::Imm(I));
        break;
      case InstructionType::kXmmXorInstruction:
        Cb.xorpd(CurrentXmmRegister, CurrentXmmRegister);
        break;
      case InstructionType::kXmmAddInstructionAlternating:
        Cb.addpd(CurrentXmmRegister, NextXmmRegister);
        break;
      case InstructionType::kXmmMovInstructionAlternating:
        Cb.movdqa(CurrentXmmRegister, NextXmmRegister);
        break;
      case InstructionType::kXmmCmpInstructionAlternating:
        Cb.cmppd(CurrentXmmRegister, NextXmmRegister, asmjit::Imm(I));
        break;
      case InstructionType::kXmmXorInstructionAlternating:
        Cb.xorpd(CurrentXmmRegister, NextXmmRegister);
        break;
      case InstructionType::kYmmAddInstruction:
        Cb.vaddpd(CurrentYmmRegister, CurrentYmmRegister, CurrentYmmRegister);
        break;
      case InstructionType::kYmmMovInstruction:
        Cb.vmovdqa(CurrentYmmRegister, CurrentYmmRegister);
        break;
      case InstructionType::kYmmCmpInstruction:
        Cb.vcmppd(CurrentYmmRegister, CurrentYmmRegister, CurrentYmmRegister, asmjit::Imm(I));
        break;
      case InstructionType::kYmmXorInstruction:
        Cb.vxorpd(CurrentYmmRegister, CurrentYmmRegister, CurrentYmmRegister);
        break;
      case InstructionType::kYmmAddInstructionAlternating:
        Cb.vaddpd(CurrentYmmRegister, NextYmmRegister, NextYmmRegister);
        break;
      case InstructionType::kYmmMovInstructionAlternating:
        Cb.vmovdqa(CurrentYmmRegister, NextYmmRegister);
        break;
      case InstructionType::kYmmCmpInstructionAlternating:
        Cb.vcmppd(CurrentYmmRegister, NextYmmRegister, NextYmmRegister, asmjit::Imm(I));
        break;
      case InstructionType::kYmmXorInstructionAlternating:
        Cb.vxorpd(CurrentYmmRegister, NextYmmRegister, NextYmmRegister);
        break;
      case InstructionType::kZmmAddInstruction:
        Cb.vaddpd(CurrentZmmRegister, CurrentZmmRegister, CurrentZmmRegister);
        break;
      case InstructionType::kZmmMovInstruction:
        Cb.vmovdqa(CurrentZmmRegister, CurrentZmmRegister);
        break;
      case InstructionType::kZmmCmpInstruction:
        Cb.vcmppd(CurrentZmmRegister, CurrentZmmRegister, CurrentZmmRegister, asmjit::Imm(I));
        break;
      case InstructionType::kZmmXorInstruction:
        Cb.vxorpd(CurrentZmmRegister, CurrentZmmRegister, CurrentZmmRegister);
        break;
      case InstructionType::kZmmAddInstructionAlternating:
        Cb.vaddpd(CurrentZmmRegister, NextZmmRegister, NextZmmRegister);
        break;
      case InstructionType::kZmmMovInstructionAlternating:
        Cb.vmovdqa(CurrentZmmRegister, NextZmmRegister);
        break;
      case InstructionType::kZmmCmpInstructionAlternating:
        Cb.vcmppd(CurrentZmmRegister, NextZmmRegister, CurrentZmmRegister, asmjit::Imm(I));
        break;
      case InstructionType::kZmmXorInstructionAlternating:
        Cb.vxorpd(CurrentZmmRegister, NextZmmRegister, NextZmmRegister);
        break;
      }
    }
  }
};

} // namespace robsize
