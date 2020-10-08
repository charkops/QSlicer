#pragma once

namespace qslicer {

  struct Point {
    float x = 0;
    float y = 0;
    float z = 0;

    Point() = default;
    Point(float x, float y, float z)
      : x (x), y (y), z (z) {};
    ~Point() = default;

    // Exact comparison
    bool operator==(const Point &p) const;
    // Floating point comparison
    bool equals(const Point &p) const;

    double dot(const Point &p) const;
    double normalize() const;
  };

  void swap(Point &, Point &);
  
  // Helper function for aboveTriangle
  float sign(const Point &, const Point &, const Point &);

} // namespace qslier