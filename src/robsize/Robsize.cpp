#include "robsize/Robsize.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <random>
#include <stdexcept>

namespace {

void createRandomLinkedListAccessPattern(std::array<void*, robsize::AddressBufferSize>& Pointers) {
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

inline auto readTimestamp() -> uint64_t {
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
    auto TestResult =
        runTest(Cfg.Start, Cfg.Stop, Cfg.UnrollCount, Cfg.InnerIterations, Cfg.OuterIterations, *Cfg.TestId);
    return RobsizeResults{.Results = TestResult};
  }

  std::vector<RobsizeResult> Results;

  for (auto I = 0; I < AvailableTests.size(); I++) {
    auto TestResults = runTest(Cfg.Start, Cfg.Stop, Cfg.UnrollCount, Cfg.InnerIterations, Cfg.OuterIterations, I);
    Results.insert(Results.end(), TestResults.cbegin(), TestResults.cend());
  }

  return RobsizeResults{.Results = Results};
}

auto RobsizeTest::runTest(unsigned Start, unsigned Stop, unsigned Unroll, unsigned InnerIterations,
                          unsigned OuterIterations, unsigned TestId) -> std::vector<RobsizeResult> {
  if (TestId >= AvailableTests.size()) {
    throw std::runtime_error("The selected test number is not available");
  }

  // Linked list of pointers that should result in a quite random access pattern.
  std::array<void*, AddressBufferSize> Pointers1{};
  std::array<void*, AddressBufferSize> Pointers2{};

  createRandomLinkedListAccessPattern(Pointers1);
  createRandomLinkedListAccessPattern(Pointers2);

  // Do a warm up.
  {
    auto Test = AvailableTests.at(TestId)->compileTest(10, /*InnerLoopRepetitions=*/InnerIterations,
                                                       /*UnrollCount=*/Unroll, /*PrintAssembler=*/false);
    for (auto I = 0; I < 10; I++) {
      Test->testFunction(Pointers1.data(), Pointers2.data());
    }
  }

  std::vector<RobsizeResult> Results;

  // Run the test for each number of filler instructions
  for (decltype(Start) InstructionCount = Start; InstructionCount <= Stop; InstructionCount++) {
    auto Test = AvailableTests.at(TestId)->compileTest(InstructionCount, /*InnerLoopRepetitions=*/InnerIterations,
                                                       /*UnrollCount=*/Unroll, /*PrintAssembler=*/false);

    uint64_t MinCyles = std::numeric_limits<uint64_t>::max();
    uint64_t MaxCycles = 0;
    uint64_t TotalCycles = 0;

    for (auto I = 0U; I < OuterIterations; I++) {
      auto StartCounter = readTimestamp();
      Test->testFunction(Pointers1.data(), Pointers2.data());
      auto StopCounter = readTimestamp();

      auto Cycles = StopCounter - StartCounter;

      MinCyles = std::min(MinCyles, Cycles);
      MaxCycles = std::max(MaxCycles, Cycles);
      TotalCycles += Cycles;
    }

    RobsizeResult Result{.TestId = TestId,
                         .InstructionCount = InstructionCount,
                         .MinCycles = MinCyles,
                         .AverageCycles = TotalCycles / OuterIterations,
                         .MaxCycles = MaxCycles};

    Results.emplace_back(Result);
  }

  return Results;
}

} // namespace robsize
