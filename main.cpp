#include <iostream>
#include <algorithm>

#include "Point.hpp"
#include "Line.hpp"
#include "Triangle.hpp"
#include "io.hpp"
#include "Writer.hpp"

using std::cout;
using std::endl;


using namespace qslicer;

std::ostream & operator<<(std::ostream &os, const Point &p) {
  return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
};

std::ostream & operator<<(std::ostream &os, const Line &l) {
  return os << l.p0 << " -> " << l.p1;
};

std::ostream & operator<<(std::ostream &os, const Triangle &t) {
  return os << t.p0 << " - " << t.p1 << " - " << t.p2;
};

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Provide an .stl file\n";
    return EXIT_FAILURE;
  }

  const std::string fileName (argv[1]);
  const float layerThickness = 0.1f;
  const float infillPercent = 0.2f;
  const float supportInfillPercentage = 0.5f;

  
  auto triangles = fileToTriangles(fileName);
  cout << "Found " << triangles.size() << " triangles\n";

  auto slices = separateSlices(triangles, layerThickness);

  
  for (auto &s : slices) {
    s = cleanPerimeter(s);
    if (s.isSurface)
      s.infill = infill(s.perimeter, 1);
    else
      s.infill = infill(s.perimeter, infillPercent);
  }
  
  auto supportSlices = generateSupports(triangles, layerThickness);
  
  for (const auto &shape : supportSlices) {
    for (std::vector<Slice>::size_type s = 0; s < shape.size(); s++) {
      slices[s].support.push_back(infill(shape[s].perimeter, supportInfillPercentage));
    }
  }
  
  writeGcode(slices, fileName);
  

  return EXIT_SUCCESS;
}