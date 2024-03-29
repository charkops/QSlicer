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
    double cross(const Point &p) const;
    bool parallel(const Point &p) const;
    double normalize() const;
  };

  void swap(Point &, Point &);
  bool close_simple(const Point &p1, const Point &p2, const double epsilon = 0.001);

  Point operator-(const Point &p1, const Point &p2);
  
  // Helper function for aboveTriangle
  float sign(const Point &, const Point &, const Point &);

} // namespace qslier