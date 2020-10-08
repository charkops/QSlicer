#pragma once

#include <algorithm>

// Some helper functionallity common to the general usage of 
// QSlicer

namespace qslicer {
  
  // For floating point comparison
  template <typename FloatingPoint>
  bool close(const FloatingPoint f1, const FloatingPoint f2, const double delta = 0.007) {
    auto comp = (std::max(f1, f2) - std::min(f1, f2));
    return (comp > -delta) && (comp < delta);
  };

} // namespace qslicer