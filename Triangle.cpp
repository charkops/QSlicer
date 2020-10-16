#include "Triangle.hpp"
#include "Line.hpp"
#include "Slice.hpp"
#include "common.hpp"

#include <algorithm>

namespace qslicer {
  bool triangleEqual(const Triangle &t1, const Triangle &t2) {
    if ((t1.p0.equals(t2.p0) && t1.p1.equals(t2.p1) && t1.p2.equals(t2.p2))
        || (t1.p0.equals(t2.p0) && t1.p1.equals(t2.p2) && t1.p2.equals(t2.p1))
        || (t1.p0.equals(t2.p1) && t1.p1.equals(t2.p0) && t1.p2.equals(t2.p2))
        || (t1.p0.equals(t2.p1) && t1.p1.equals(t2.p2) && t1.p2.equals(t2.p0))
        || (t1.p0.equals(t2.p2) && t1.p1.equals(t2.p0) && t1.p2.equals(t2.p1))
        || (t1.p0.equals(t2.p2) && t1.p1.equals(t2.p1) && t1.p2.equals(t2.p0)))
        return true;
    else
        return false;
  };

  // given a vector of triangles in 3D space,
  // returns a pair of the highest and lowest Z values
  // TODO (@charkops): Refactor this custom min, max algorith with STL
  std::pair<float, float> findBoundaries(const std::vector<Triangle> &triangles) {
    float bottomZ = std::numeric_limits<float>::max();
    float topZ = std::numeric_limits<float>::min();

    for (const auto &triangle : triangles) {
      auto maximum = std::max(std::max(triangle.p0.z, triangle.p1.z), triangle.p2.z);
      auto minimum = std::min(std::min(triangle.p0.z, triangle.p1.z), triangle.p2.z);

      if (maximum > topZ)
        topZ = maximum;
      if (minimum < bottomZ)
        bottomZ = minimum;
    }

    return std::make_pair(bottomZ, topZ);
  };

  // given a vector of triangles and a thickness per layer,
  // computes the total number of layers and checks which
  // line segments need to be drawn in each layer,
  // returns the vector of slices with the vector of line segments
  // to draw per slice, as a pair with a bool for if the slice 
  // is a bottom or top
  std::vector<Slice> separateSlices(const std::vector<Triangle> &triangles, const float layerThickness) {
    auto bounds = findBoundaries(triangles);
    int numSlices = static_cast<int>((std::get<1>(bounds) - std::get<0>(bounds)) / layerThickness);
    std::vector<float> slices;
    for (int z = 0; z < numSlices + 1; ++z) {
      slices.push_back(std::get<0>(bounds) + z * layerThickness);
    }

    // What is segments
    std::vector<Slice> segments;
    for (auto s : slices) {
      std::vector<Line> currentSegment;
      auto currentSegmentSurface = false;
      for (const auto &triangle : triangles) {
        auto point1 = Line(triangle.p0, triangle.p1).intersectSlice(s);
        auto point2 = Line(triangle.p1, triangle.p2).intersectSlice(s);
        auto point3 = Line(triangle.p2, triangle.p0).intersectSlice(s);


        // Create a vector of non - empty points ?
        std::vector<Point> points_;
        if (point1)
          points_.push_back(*point1);
        if (point2)
          points_.push_back(*point2);
        if (point3)
          points_.push_back(*point3);

        // NOTE (@charkops): Insert unique points to vec ? Why not use std::set ?
        std::vector<Point> points;
        for (std::vector<Point>::size_type i = 0; i < points_.size(); i++) {
          auto j = i + 1;
          auto unique = true;
          while (j < points_.size()) {
            if (points_[i].equals(points_[j]))
              unique = false;
            j++;
          }
          if (unique) {
            points.insert(points.begin(), points_[i]);
          }
        }
        if (s <= (std::get<0>(bounds) + layerThickness) || (s >= (std::get<1>(bounds) - layerThickness)))
          currentSegmentSurface = true;

        if (points.size() == 2) {
          currentSegment.push_back(Line(points[0], points[1]));
        } else if (points.size() == 3) {
          auto segment1 = Line(points[0], points[1]);
          auto segment2 = Line(points[1], points[2]);
          auto segment3 = Line(points[2], points[0]);
          currentSegmentSurface = true;

          currentSegment.push_back(segment1);
          currentSegment.push_back(segment2);
          currentSegment.push_back(segment3);
        }

      }
      segments.push_back(Slice(s, currentSegment, currentSegmentSurface));
    }

    return segments;
  };

  bool aboveTriangle(const Point &p, const Triangle &triangle, const float delta) {
    if (p.z > (triangle.p0.z - delta) &&
        p.z > (triangle.p1.z - delta) &&
        p.z > (triangle.p2.z - delta)) 
    {
      auto b1 = (sign(p, triangle.p0, triangle.p1) < 0.0);
      auto b2 = (sign(p, triangle.p1, triangle.p2) < 0.0);
      auto b3 = (sign(p, triangle.p2, triangle.p0) < 0.0);

      return ((b1 == b2) && (b2 == b3));
    }
    else 
    {
      return false;
    }
  };

  std::vector<Triangle> downwardTriangles(const std::vector<Triangle> &triangles) { 
    std::vector<Triangle> trianglesDown;
    for (const auto &triangle : triangles)
      if (triangle.norm.z < 0)
        trianglesDown.insert(trianglesDown.begin(), triangle);

    return trianglesDown;
  };

  bool supportNeeded(const Triangle &triangle, const std::vector<Triangle> &triangles, const float bottomZ) {
    if (close(triangle.p0.z, bottomZ) && close(triangle.p1.z, bottomZ) && close(triangle.p2.z, bottomZ))
      return false;
    for (const auto &tri : triangles) {
      if (aboveTriangle(triangle.p0, tri) || aboveTriangle(triangle.p1, tri) || aboveTriangle(triangle.p2, tri))
        return false;
    }

    return true;
  };

  std::vector<Triangle> generateSupportShape(const Triangle &triangle, const float bottomZ) {
    auto triangleTop = triangle;
    auto triangleBottom = Triangle(Point(triangleTop.p0.x, triangleTop.p0.y, bottomZ),
                                    Point(triangleTop.p1.x, triangleTop.p1.y, bottomZ),
                                    Point(triangleTop.p2.x, triangleTop.p2.y, bottomZ),
                                    false);
    
    std::vector<Triangle> newShape = {triangleTop};
    newShape.insert(newShape.begin(), triangleBottom);
    newShape.insert(newShape.begin(), Triangle(triangleTop.p0, triangleTop.p1, triangleBottom.p0, false));
    newShape.insert(newShape.begin(), Triangle(triangleTop.p1, triangleTop.p2, triangleBottom.p1, false));
    newShape.insert(newShape.begin(), Triangle(triangleTop.p2, triangleTop.p0, triangleBottom.p2, false));
    newShape.insert(newShape.begin(), Triangle(triangleBottom.p0, triangleBottom.p1, triangleTop.p1, false));
    newShape.insert(newShape.begin(), Triangle(triangleBottom.p1, triangleBottom.p2, triangleTop.p2, false));
    newShape.insert(newShape.begin(), Triangle(triangleBottom.p2, triangleBottom.p0, triangleTop.p0, false));
  
    std::vector<Triangle>::size_type i = 0;
    while (i < newShape.size()) {
      auto j = i + 1;
      while (j < newShape.size()) {
        if (triangleEqual(newShape[i], newShape[j]))
          // TODO (@charkops): This should be newShape[j]
          // I am not sure if newShape.begin() + j == newShape[j]
          // Please check in the future
          newShape.erase(newShape.begin() + j);
        else 
          j++;
      }
      i++;
    }

    return newShape;
  };

  std::vector<std::vector<Slice>> generateSupports (const std::vector<Triangle> &triangles, const float layerThickness) {
    auto bounds = findBoundaries(triangles);
    auto trianglesDown = downwardTriangles(triangles);

    std::vector<Triangle> trianglesForSupport;
    for (const auto &tri : trianglesDown) {
      if (supportNeeded(tri, triangles, std::get<0>(bounds)))
        trianglesForSupport.insert(trianglesForSupport.begin(), tri);
    }

    std::vector<std::vector<Triangle>> supportShapes;
    for (const auto &triangle : trianglesForSupport)
      supportShapes.insert(supportShapes.begin(), generateSupportShape(triangle, std::get<0>(bounds)));
    

    std::vector<std::vector<Slice>> supportSlices;
    for (const auto &shape : supportShapes)
      supportSlices.insert(supportSlices.begin(), separateSlices(shape, layerThickness));

    return supportSlices;
  };

} // namespace qslicer