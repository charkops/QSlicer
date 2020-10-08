#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "Triangle.hpp"

namespace qslicer {
  // Parses an .stl file and returns a vector of Triangles
  std::vector<Triangle> fileToTriangles(const std::string &fileName);
  Triangle readFacet(std::ifstream &file);
} // namespace qslicer