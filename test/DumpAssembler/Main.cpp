#include "robsize/Robsize.hpp"

namespace {

/// Dump the generated assembler code of the test.
void dumpTest(const std::shared_ptr<robsize::CacheMissTest>& Test) {
  (void)Test->compileTest(/*InstructionCount=*/10, /*InnerLoopRepetitions=*/2, /*UnrollCount=*/3,
                          /*PrintAssembler=*/true);
}

} // namespace

auto main(int /*argc*/, const char** /*argv*/) -> int {
  auto Robsize = robsize::RobsizeTest();

  for (const auto& Test : Robsize.avaialableTests()) {
    std::cout << Test->name() << "\n";
    dumpTest(Test);
  }

  return EXIT_SUCCESS;
}