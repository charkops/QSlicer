#include "Line.hpp"
#include "common.hpp"

namespace qslicer {
  void Line::reverse() {
    swap(p0, p1);
  };

  // bool Line::operator==(const Line &l) {
  //   return 
  // };

  // Wtf does this even do ?
  bool lineEqual(const Line &l1, const Line &l2) {
    if ((close(l1.p0.x, l2.p0.x) && close(l1.p0.y, l2.p0.y) 
      && close(l1.p1.x, l2.p1.x) && close(l1.p1.y, l2.p1.y))
      || (close(l1.p0.x, l2.p1.x) && close(l1.p0.y, l2.p1.y)
      && close(l1.p1.x, l2.p0.x) && close(l1.p1.y, l2.p0.y)))
    {
        return true;
    } else {
      return false;
    }
  };

  // given a line segment and a plane,
  // returns the point at which those two
  // planes intersect. Will return the first point
  // in the line if the whole line is in the plane
  std::optional<Point> Line::intersectSlice(const float plane) {
    if ((p0.z == p1.z) && (p1.z == plane)) {
      return p0;
    }
    else if (p0.z == p1.z) {
      return std::nullopt;
    }
    else {
      auto slope = Point(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
      auto t = static_cast<float>(plane - p0.z) / slope.z;

      if (t >= 0 && t <= 1) {
        auto testZ = p0.z + t*slope.z;
        if ((testZ <= std::max(p0.z, p1.z)) && (testZ >= std::min(p0.z, p1.z))) {
          return Point(p0.x + t*slope.x, p0.y + t*slope.y, p0.z + t*slope.z);
        } else {
          return std::nullopt;
        }
      } else {
        return std::nullopt;
      }
    }
  };

  std::optional<Point> intersection(const Line &l1, const Line &l2, const float delta) {
    auto x1 = l1.p0.x;
    auto y1 = l1.p0.y;
    auto x2 = l1.p1.x;
    auto y2 = l1.p1.y;
    auto x3 = l2.p0.x;
    auto y3 = l2.p0.y;
    auto x4 = l2.p1.x;
    auto y4 = l2.p1.y;

    auto xnum = (x1 * y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4);
    auto xden = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    auto ynum = (x1*y2-y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    auto yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);

    try {
      auto intersect = Point(xnum / xden, ynum / yden, l1.p0.z);
      if ((intersect.x >= std::min(x1,x2)-delta) && (intersect.x <= std::max(x1,x2)+delta) &&
            (intersect.y >= std::min(y1,y2)-delta) && (intersect.y <= std::max(y1,y2)+delta) &&
            (intersect.x >= std::min(x3,x4)-delta) && (intersect.x <= std::max(x3,x4)+delta) &&
            (intersect.y >= std::min(y3,y4)-delta) && (intersect.y <= std::max(y3,y4)+delta))
        return intersect;
      else
        return std::nullopt;
    } catch (const std::exception &e) {
      return std::nullopt;
    }

  };

  bool pointInLine(const Point &p, const Line &line) {
    if (close(p.x, line.p0.x) && close(p.y, line.p0.y) && close(p.z, line.p0.z))
      return true;
    else if (close(p.x, line.p1.x) && close(p.y, line.p1.y) && close(p.z, line.p1.z))
      return true;
    else
      return false;
  };

} // namespace qslicer