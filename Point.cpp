#include "Point.hpp"

#include "common.hpp"

#include <cmath> // for std::sqrt
#include <utility> // std::swap

namespace qslicer {

  double Point::dot(const Point &p) const {
    return x * p.x + y * p.y + z * p.z;
  };

  double Point::cross(const Point &p) const {
    return Point(y*p.z - z*p.y, x*p.z - z*p.x, x*p.y - y*p.x).normalize();
  };

  bool Point::parallel(const Point &p) const {
    return this->cross(p) == 0;
  };

  double Point::normalize() const {
    return std::sqrt(x*x + y*y + z*z);
  };

  bool Point::operator==(const Point &p) const {
    return (x == p.x) && (y == p.y) && (z == p.z);
  };

  bool Point::equals(const Point &p) const {
    if (close(x, p.x) && close(y, p.y) && close(z, p.z))
      return true;
    return false;
  };

  bool close_simple(const Point &p1, const Point &p2, const double epsilon) {
    return close_simple(p1.x, p2.x, epsilon) && close_simple(p1.y, p2.y, epsilon) && close_simple(p1.z, p2.z, epsilon);
  };

  void swap(Point &p1, Point &p2) {
    using std::swap;
    swap(p1.x, p2.x);
    swap(p1.y, p2.y);
    swap(p1.z, p2.z);
  };

  Point operator-(const Point &p1, const Point &p2) {
    return Point(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
  };

  float sign(const Point &p1, const Point &p2, const Point &p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
  };

} // namespace qslicer