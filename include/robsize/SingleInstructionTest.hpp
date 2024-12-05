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
    return "nop;single-byte";
  case InstructionType::kTwoByteNopInstruction:
    return "nop;two-byte";
  case InstructionType::kAddInstruction:
    return "add;gp-registers;non-alternating";
  case InstructionType::kMovInstruction:
    return "mov;gp-registers;non-alternating";
  case InstructionType::kCmpInstruction:
    return "cmp;gp-registers;non-alternating";
  case InstructionType::kXorInstruction:
    return "xor;gp-registers;non-alternating";
  case InstructionType::kAddInstructionAlternating:
    return "add;gp-registers;alternating";
  case InstructionType::kMovInstructionAlternating:
    return "mov;gp-registers;alternating";
  case InstructionType::kCmpInstructionAlternating:
    return "cmp;gp-registers;alternating";
  case InstructionType::kXorInstructionAlternating:
    return "xor;gp-registers;alternating";
  case InstructionType::kXmmAddInstruction:
    return "add;xmm-registers;non-alternating";
  case InstructionType::kXmmMovInstruction:
    return "mov;xmm-registers;non-alternating";
  case InstructionType::kXmmCmpInstruction:
    return "cmp;xmm-registers;non-alternating";
  case InstructionType::kXmmXorInstruction:
    return "xor;xmm-registers;non-alternating";
  case InstructionType::kXmmAddInstructionAlternating:
    return "add;xmm-registers;alternating";
  case InstructionType::kXmmMovInstructionAlternating:
    return "mov;xmm-registers;alternating";
  case InstructionType::kXmmCmpInstructionAlternating:
    return "cmp;xmm-registers;alternating";
  case InstructionType::kXmmXorInstructionAlternating:
    return "xor;xmm-registers;alternating";
  case InstructionType::kYmmAddInstruction:
    return "add;ymm-registers;non-alternating";
  case InstructionType::kYmmMovInstruction:
    return "mov;ymm-registers;non-alternating";
  case InstructionType::kYmmCmpInstruction:
    return "cmp;ymm-registers;non-alternating";
  case InstructionType::kYmmXorInstruction:
    return "xor;ymm-registers;non-alternating";
  case InstructionType::kYmmAddInstructionAlternating:
    return "add;ymm-registers;alternating";
  case InstructionType::kYmmMovInstructionAlternating:
    return "mov;ymm-registers;alternating";
  case InstructionType::kYmmCmpInstructionAlternating:
    return "cmp;ymm-registers;alternating";
  case InstructionType::kYmmXorInstructionAlternating:
    return "xor;ymm-registers;alternating";
  case InstructionType::kZmmAddInstruction:
    return "add;zmm-registers;non-alternating";
  case InstructionType::kZmmMovInstruction:
    return "mov;zmm-registers;non-alternating";
  case InstructionType::kZmmCmpInstruction:
    return "cmp;zmm-registers;non-alternating";
  case InstructionType::kZmmXorInstruction:
    return "xor;zmm-registers;non-alternating";
  case InstructionType::kZmmAddInstructionAlternating:
    return "add;zmm-registers;alternating";
  case InstructionType::kZmmMovInstructionAlternating:
    return "mov;zmm-registers;alternating";
  case InstructionType::kZmmCmpInstructionAlternating:
    return "cmp;zmm-registers;alternating";
  case InstructionType::kZmmXorInstructionAlternating:
    return "xor;zmm-registers;alternating";
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

  /// Method to get the requried stack size for the test
  /// \returns The required stack size
  auto requiredStackSize() -> unsigned final {
    // None of the tests need more stack size
    return 0;
  }
};

} // namespace robsize
