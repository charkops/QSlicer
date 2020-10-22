#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "Triangle.hpp"

namespace qslicer {
  // Parses an .stl file and returns a vector of Triangles
  // Reads both ascii && binary files
  // Throws a std::exception if cannot read file
  std::vector<Triangle> fileToTriangles(const std::string &fileName);
  
} // namespace qslicer