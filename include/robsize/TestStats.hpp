#pragma once

#include <cstdint>

namespace robsize {

/// The stats of the compiled test.
struct TestStats {
  /// The size of the loop in bytes.
  uint64_t LoopSizeB;
};

} // namespace robsize