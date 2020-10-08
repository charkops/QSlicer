#pragma once 

#include <utility>
#include <vector>

#include "Point.hpp"
#include "Slice.hpp"

namespace qslicer {
  struct Triangle {
    Point p0, p1, p2, norm;
    bool hasNorm = true;

    Triangle() = default;
    Triangle(const Point &p0, const Point &p1, const Point &p2, const Point &norm)
      : p0 (p0), p1 (p1), p2 (p2), norm (norm) {};
    
    Triangle(const Point &p0, const Point &p1, const Point &p2, bool hasNorm)
      : p0 (p0), p1 (p1), p2 (p2), hasNorm (hasNorm) {};

    ~Triangle() = default;
  };

  bool triangleEqual(const Triangle &, const Triangle &);
  std::pair<float, float> findBoundaries(const std::vector<Triangle> &);

  std::vector<Slice> separateSlices(const std::vector<Triangle> &triangles, const float layerThickness);

  // given a point and a triangle, 
  // returns True if that point is directly above that triangle,
  // False otherwise
  bool aboveTriangle(const Point &, const Triangle &, const float delta = 0.007);

  // Given a vector of triangles
  // Returns a vector of downward-facing triangles
  std::vector<Triangle> downwardTriangles(const std::vector<Triangle> &);

  // given a downward-facing triangle and a list of all triangles,
  // returns True if no triangles are in the way of the downward triangle
  // and False if a triangle blocks the path directly downward
  bool supportNeeded(const Triangle &, const std::vector<Triangle> &, const float);

  // given a triangle that requires support and a minimum Z value,
  // returns the list of triangles required to form the support shape under that triangle
  std::vector<Triangle> generateSupportShape(const Triangle &, const float);

  // TODO (@charkops): Implement generate support

} // namespace qslicer