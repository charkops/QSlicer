#pragma once

#include <algorithm>
#include <cmath>

// Some helper functionallity common to the general usage of 
// QSlicer

namespace qslicer {
  
  // For floating point comparison
  template <typename FloatingPoint>
  inline bool close(const FloatingPoint f1, const FloatingPoint f2, const double delta = 0.007) {
    auto comp = (std::max(f1, f2) - std::min(f1, f2));
    return (comp > -delta) && (comp < delta);
  };

  // For floating point comparison
  template <typename FloatingPoint>
  inline bool close_simple(const FloatingPoint f1, const FloatingPoint f2, const double epsilon = 0.001) {
    return std::abs(f1 - f2) < epsilon;
  }

} // namespace qslicer