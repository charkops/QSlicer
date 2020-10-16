#pragma once

#include "Point.hpp"

#include <optional>
#include <vector>

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

    bool parallel(const Line &line) const;

    bool operator==(const Line &line) const;

    std::optional<Point> intersectSlice(const float);
  };

  bool lineEqual(const Line &, const Line &, const double delta = 0.007);

  // "Simply" determines if 2 lines are "close" to each other
  bool lineCloseSimple(const Line&, const Line&, const double epsilon = 0.001);

  // Checks to see if 2 lines can be concatenated, if they can, returns the new line segment
  std::optional<Line> concat (const Line &, const Line &);

  // given two lines on the same z plane, 
  // returns the point at which they intersect,
  // or None if there is no intersection
  std::optional<Point> intersection(const Line &, const Line &, const float delta = 0.007);

  bool pointInLine(const Point &, const Line &);

  // Given a vector of lines that make a manifold perimeter on a slice
  // and a percentage of space that should be infill 
  // Returns a vector of lines (grid pattern) for that slice
  // Assumes print bed area is a square
  std::vector<Line> infill (const std::vector<Line> &perimeter, const float percent, const float bedWidth = 150.00f, const float extrudeWidth = 0.007f);

} // namespace qslicer
