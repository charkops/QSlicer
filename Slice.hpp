#pragma once

#include <vector>

#include "Line.hpp"

namespace qslicer {
  struct Slice {
    float zvalue = 0.0f;
    std::vector<Line> perimeter = {};
    bool isSurface = false;

    Slice() = default;
    Slice(float zvalue, const std::vector<Line> &perimeter, bool isSurface)
      : zvalue (zvalue), perimeter (perimeter), isSurface (isSurface) {};
    ~Slice() = default;
  };
} // namespace qslicer