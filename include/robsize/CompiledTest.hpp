#pragma once

#include "robsize/TestStats.hpp"

#include <asmjit/asmjit.h>
#include <memory>
#include <stdexcept>

namespace robsize {

/// This class represents a test that can be executed. It is created by calling compileTest of the CacheMissTest class
/// with specific settings.
class CompiledTest {
public:
  CompiledTest() = delete;
  virtual ~CompiledTest() = default;

  /// A unique ptr for the CompiledPayload with a custom deleter.
  using UniquePtr = std::unique_ptr<CompiledTest, void (*)(CompiledTest*)>;

  /// The function take two arrays of pointers (linked lists).
  using TestFunctionPtr = void (*)(void**, void**);

  /// Create an executable test from generated assembler code.
  /// \arg Stats The stats of the test that is generated from the assembler code.
  /// \arg Code The CodeHolder that contains the assembler code
  /// \returns A unique pointer to this class that allows executing the test.
  [[nodiscard]] static auto create(TestStats Stats, asmjit::CodeHolder& Code) -> UniquePtr {
    TestFunctionPtr TestFunction{};
    const auto Err = Runtime.add(&TestFunction, &Code);
    if (Err) {
      throw std::runtime_error("Asmjit adding Assembler to JitRuntime failed");
    }

    return {new CompiledTest(Stats, TestFunction), deleter};
  }

  /// Getter for the stats of compiled test.
  [[nodiscard]] auto stats() const -> const TestStats& { return Stats; };

  void testFunction(void** LinkedList1, void** LinkedList2) const { TestFunction(LinkedList1, LinkedList2); }

private:
  /// Constructor for the CompiledPayload.
  /// \arg Stats The stats of the compiled test.
  /// \arg TestFunction The pointer to the compiled test function.
  CompiledTest(const TestStats& Stats, TestFunctionPtr TestFunction)
      : Stats(Stats)
      , TestFunction(TestFunction) {}

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  inline static asmjit::JitRuntime Runtime = asmjit::JitRuntime();

  /// Custom deleter to release the memory of the high load function from the asmjit runtime.
  /// \arg Payload The pointer to this class
  static void deleter(CompiledTest* Test) {
    if (Test && Test->TestFunction) {
      Runtime.release(Test->TestFunction);
    }
  }

  /// The stats of the compiled test.
  TestStats Stats;
  /// The pointer to the compiled test function.
  TestFunctionPtr TestFunction;
};

} // namespace robsize