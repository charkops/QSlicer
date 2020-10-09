#pragma once

#include "Slice.hpp"
#include <string>

namespace qslicer {

  std::string gcodeSuffix(const std::string &fileName);

  void writeGcode(const std::vector<Slice> &slices, const std::string &fileName, const float extrudeRate = 0.05, const float bedWidth = 150.0f);
} // namespace qslicer