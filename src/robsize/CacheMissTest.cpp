#include "robsize/CacheMissTest.hpp"
#include "robsize/TestStats.hpp"

#include <asmjit/asmjit.h>

namespace robsize {

auto CacheMissTest::compileTest(unsigned InstructionCount, unsigned InnerLoopRepetitions, unsigned UnrollCount)
    -> CompiledTest::UniquePtr {
  asmjit::CodeHolder Code;
  Code.init(asmjit::Environment::host());

  asmjit::x86::Builder Cb(&Code);
  Cb.addDiagnosticOptions(asmjit::DiagnosticOptions::kValidateAssembler |
                          asmjit::DiagnosticOptions::kValidateIntermediate);

  const auto PointerReg1 = asmjit::x86::rax;
  const auto PointerReg2 = asmjit::x86::rbx;
  const auto CounterReg = asmjit::x86::rcx;

  const std::vector<asmjit::x86::Gpq> AvailableGpRegisters = {
      asmjit::x86::r8,  asmjit::x86::r9,  asmjit::x86::r10, asmjit::x86::r11, asmjit::x86::r12, asmjit::x86::r13,
      asmjit::x86::r14, asmjit::x86::r15, asmjit::x86::rdi, asmjit::x86::rsi, asmjit::x86::rdx};

  asmjit::FuncDetail Func;
  Func.init(asmjit::FuncSignature::build<void, void**, void**>(asmjit::CallConvId::kCDecl), Code.environment());

  asmjit::FuncFrame Frame;
  Frame.init(Func);
  // The registers used to pass arguments do not need to be marked dirty.
  Frame.addDirtyRegs(CounterReg);
  for (const auto& Reg : AvailableGpRegisters) {
    Frame.addDirtyRegs(Reg);
  }
  // Make all Zmm/Ymm/Xmm registers dirty and available for the filler instructions.
  for (auto I = 0U; I < 32; I++) {
    Frame.addDirtyRegs(asmjit::x86::Zmm(I));
  }

  asmjit::FuncArgsAssignment Args(&Func);
  Args.assignAll(PointerReg1, PointerReg2);
  Args.updateFuncFrame(Frame);
  Frame.finalize();

  Cb.emitProlog(Frame);
  Cb.emitArgsAssignment(Frame, Args);

  // The actual code for the cache miss test
  Cb.mov(CounterReg, asmjit::Imm(InnerLoopRepetitions));

  auto LoopStart = Cb.newLabel();
  Cb.bind(LoopStart);

  for (auto I = 0U; I < UnrollCount; I++) {
    // First cache miss
    Cb.mov(PointerReg1, asmjit::x86::ptr(PointerReg1));

    addFillerInstructions(Cb, InstructionCount, AvailableGpRegisters);

    // Second cache miss
    Cb.mov(PointerReg2, asmjit::x86::ptr(PointerReg2));

    Cb.lfence();
  }

  Cb.sub(CounterReg, asmjit::Imm(1));
  Cb.jnz(LoopStart);

  auto FunctionExit = Cb.newLabel();
  Cb.bind(FunctionExit);

  Cb.emitEpilog(Frame);

  Cb.finalize();

  auto LoopSize = Code.labelOffset(FunctionExit) - Code.labelOffset(LoopStart);

  TestStats Stats{.LoopSizeB = LoopSize};

  return CompiledTest::create(Stats, Code);
}

} // namespace robsize