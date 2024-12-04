#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace robsize {

struct RobsizeResult {
  unsigned TestId;
  std::string TestName;
  unsigned InstructionCount;
  uint64_t MinCycles;
  uint64_t AverageCycles;
  uint64_t MaxCycles;
};

struct RobsizeResults {
  RobsizeResults() = default;

  std::vector<RobsizeResult> Results;

  /// Save the results as a CSV file.
  /// \arg FilePath The path of the file where the CSV should be saved.
  void saveCsv(const std::string& Filepath) {
    std::ofstream Outfile(Filepath);

    if (Outfile.bad()) {
      throw std::runtime_error("I/O error while writing CSV");
    }

    Outfile << "TestId,TestName,InstructionCount,MinCycles,AverageCycles,MaxCycles\n";

    for (const auto& Entry : Results) {
      Outfile << Entry.TestId << "," << Entry.TestName << "," << Entry.InstructionCount << "," << Entry.MinCycles << ","
              << Entry.AverageCycles << "," << Entry.MaxCycles << "\n";
    }
  }
};

} // namespace robsize