#include "robsize/Robsize.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <random>
#include <stdexcept>

namespace {

constexpr const std::size_t AddressBufferSize = 8192;

void createRandomLinkedListAccessPattern(std::array<void*, AddressBufferSize>& Pointers) {
  // Write the address of each element to the pointer array and shuffle it around to create (ideally one) chains of
  // pointers. Increment the AddressBufferSize to a number big enough to create a long enough chain.
  for (auto& Elem : Pointers) {
    Elem = static_cast<void*>(&Elem);
  }

  // TODO: use constant seed
  std::random_device Rd;
  std::mt19937 Rng(Rd());
  std::shuffle(Pointers.begin(), Pointers.end(), Rng);
}

auto readTimestamp() -> uint64_t {
  // NOLINTBEGIN(misc-const-correctness)
  uint64_t Rax = 0;
  uint64_t Rdx = 0;
  // NOLINTEND(misc-const-correctness)
  __asm__ __volatile__("rdtsc;" : "=a"(Rax), "=d"(Rdx));
  return (Rdx << 32) | (Rax & 0xffffffffULL);
}

} // namespace

namespace robsize {

auto RobsizeTest::runTests(const robsize::Config& Cfg) -> RobsizeResults {
  // Run a single selected test
  if (Cfg.TestId) {
    auto TestResult = runTest(Cfg.Start, Cfg.Stop, *Cfg.TestId);
    return RobsizeResults(TestResult);
  }

  RobsizeResults Results;

  for (auto I = 0; I < AvailableTests.size(); I++) {
    auto TestResult = runTest(Cfg.Start, Cfg.Stop, I);
    Results.TestResults.emplace_back(TestResult);
  }

  return Results;
}

auto RobsizeTest::runTest(unsigned Start, unsigned Stop, unsigned TestId) -> RobsizeResult {
  if (TestId >= AvailableTests.size()) {
    throw std::runtime_error("The selected test number is not available");
  }

  // Linked list of pointers that should result in a quite random access pattern.
  std::array<void*, AddressBufferSize> Pointers1{};
  std::array<void*, AddressBufferSize> Pointers2{};

  createRandomLinkedListAccessPattern(Pointers1);
  createRandomLinkedListAccessPattern(Pointers2);

  RobsizeResult Result;
  Result.TestId = TestId;

  // Run the test for each number of filler instructions
  for (decltype(Start) InstructionCount = Start; InstructionCount <= Stop; InstructionCount++) {
    auto Test =
        AvailableTests.at(TestId)->compileTest(InstructionCount, /*InnerLoopRepetitions=*/1024, /*UnrollCount=*/16);

    // TODO: run multiple iterations.
    auto StartCounter = readTimestamp();
    Test->testFunction(Pointers1.data(), Pointers2.data());
    auto StopCounter = readTimestamp();

    Result.InstructionCountResults[InstructionCount] = InstructionCountResult{.Cycles = StopCounter - StartCounter};
  }

  return Result;
}

} // namespace robsize
