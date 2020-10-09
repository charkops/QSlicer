#include "Line.hpp"
#include "common.hpp"

#include <stdexcept>
#include <cmath>

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

  std::vector<Line> infill (const std::vector<Line> &perimeter, const float percent, const float bedWidth, const float extrudeWidth) {
    if (percent < 0 || percent > 1)
      throw std::out_of_range("Percent out of range");

    if (perimeter.empty())
      return {};
    
    // Z should be the same across all lines
    auto Z = perimeter[0].p0.z;
    unsigned int numLines = static_cast<unsigned int>(std::round((bedWidth*percent) / extrudeWidth));
    auto gap = bedWidth / numLines;

    std::vector<Line> infill;
    for (unsigned int x = 0; x < numLines; ++x) {
      auto fullLine = Line(Point((bedWidth / (-2)) + (x*gap), bedWidth / (-2), Z), Point((bedWidth / (-2)) + (x*gap), bedWidth / 2, Z));

      std::vector<Point> inters;

      // Find intersections without repeats
      for (const auto &line : perimeter) {
        auto sect = intersection(line, fullLine);
        if (sect) {
          auto newVal = true;
          for (const auto &i : inters) {
            if (close(i.y, (*sect).y))
              newVal = false;
          }
          if (newVal)
            inters.push_back(*sect);
        }
      }

      // Sort by y to get matching pairs
      std::sort(inters.begin(), inters.end(), [](const Point &p1, const Point &p2) {
        return p1.y < p2.y;
      });

      // If not even, something went wrong and its safer not to print
      if (inters.size() % 2 == 0) {
        for (std::vector<Point>::size_type i = 0; i < inters.size(); ++i) {
          if (i % 2 != 0) {
            auto overlap = false;
            auto newLine = Line(inters[i - 1], inters[i]);
            for (const auto &l : perimeter) {
              if (lineEqual(l, newLine))
                overlap = true;
            }
            if (!overlap) {
              infill.push_back(newLine);
            }
          }
        }
      }
    }

    return infill;
  };

} // namespace qslicer