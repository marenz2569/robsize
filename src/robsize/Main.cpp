#include "robsize/Config.hpp"
#include "robsize/Robsize.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <stdexcept>
#include <sys/resource.h>

namespace {

void checkStackSize() {
  struct rlimit Limit {};
  getrlimit(RLIMIT_STACK, &Limit);
  if (Limit.rlim_cur < 2048 * robsize::AddressBufferSize) {
    throw std::runtime_error(
        "The stack size is too small, increase it or set it to unlimited with: ulimit -s unlimited\n");
  }
}

/// Set the affinity to the supplied os cpu index.
/// \arg Cpu The cpu index to which this thread should be bound to.
void setAffinity(std::size_t Cpu) {
  const pthread_t Thread = pthread_self();
  cpu_set_t Cpus;

  CPU_ZERO(&Cpus);
  CPU_SET(Cpu, &Cpus);

  pthread_setaffinity_np(Thread, sizeof(Cpus), &Cpus);
}

} // namespace

auto main(int Argc, const char** Argv) -> int {
  std::cout << "robsize - Microarchitectural benchmark to measure the reorder buffer size based on the methodoly "
               "described by Henry Wong, Version "
            << _ROBSIZE_VERSION_STRING << "\n"
            << "Copyright (C) " << _ROBSIZE_BUILD_YEAR << " Markus Schmidl" << "\n";

  try {
    const robsize::Config Cfg{Argc, Argv};

    robsize::RobsizeTest Robsize;

    if (Cfg.PrintTests) {
      std::cout << "Index TestName\n";
      auto I = 0U;
      for (const auto& Test : Robsize.avaialableTests()) {
        std::cout << I++ << " " << Test->name() << "\n";
      }
      return EXIT_SUCCESS;
    }

    checkStackSize();

    setAffinity(Cfg.CpuIndex);

    auto Results = Robsize.runTests(Cfg);

    Results.saveCsv(Cfg.OutfilePath);
  } catch (std::exception const& E) {
    std::cerr << E.what();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}