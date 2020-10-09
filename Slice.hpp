#pragma once

#include <vector>

#include "Line.hpp"

namespace qslicer {
  struct Slice {
    float zvalue = 0.0f;
    std::vector<Line> perimeter = {};
    bool isSurface = false;
    std::vector<Line> infill;
    std::vector<std::vector<Line>> support;

    Slice() = default;
    Slice(float zvalue, const std::vector<Line> &perimeter, bool isSurface)
      : zvalue (zvalue), perimeter (perimeter), isSurface (isSurface) {};
    ~Slice() = default;
  };

  // Given a slice with a vector of line segments
  // Returns a new slice free of duplicate of interior line segments
  // and in order for optimized drawing
  Slice cleanPerimeter(const Slice &);

} // namespace qslicer