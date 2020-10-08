#pragma once

#include "Point.hpp"

#include <optional>

namespace qslicer {
  
  struct Line {
    Point p0;
    Point p1;

    Line() = default;
    Line(const Point &p0, const Point &p1)
      : p0 (p0), p1 (p1) {};

    ~Line() = default;

    // Swaps p0 and p1
    void reverse();

    std::optional<Point> intersectSlice(const float);
  };

  bool lineEqual(const Line &, const Line &);

  // given two lines on the same z plane, 
  // returns the point at which they intersect,
  // or None if there is no intersection
  std::optional<Point> intersection(const Line &, const Line &, const float delta = 0.007);

  bool pointInLine(const Point &, const Line &);

} // namespace qslicer
