#include <iostream>
#include <algorithm>

#include "Point.hpp"
#include "Line.hpp"
#include "Triangle.hpp"
#include "io.hpp"

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

  auto triangles = fileToTriangles(fileName);
  cout << "Found " << triangles.size() << " triangles\n";


  return EXIT_SUCCESS;
}