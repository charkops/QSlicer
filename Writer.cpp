#include "Writer.hpp"

#include <fstream>
#include <cmath>

namespace qslicer {
  std::string gcodeSuffix(const std::string &fileName) {
    return fileName.substr(0, fileName.length() - 3) + "gcode";
  };

  void writeGcode(const std::vector<Slice> &slices, const std::string &fileName, const float extrudeRate, const float bedWidth) {
    // Get gcode Extension
    auto outFile = gcodeSuffix(fileName);
    std::ofstream file (outFile, std::ios::out);
    if (!file.good())
      throw std::runtime_error("Could not open file: " + outFile);

    // Preamble
    file << ";Start GCode\n";
    file << "M109 S210\n";
    file << "G28 X0 Y0 Z0\n";
    file << "G92 E0\n";
    file << "G29\n";

    auto origin = bedWidth / 2;
    unsigned int layer = 1; // Current layer
    double E = 0.0;       // Extrusion accumulator

    file << ";LAYER_COUNT: " << std::to_string(slices.size()) << '\n';
    for (const auto s : slices) {
      file << ";LAYER: " << std::to_string(layer) << '\n';

      // Fan 
      if (layer == 2) {
        file << "M106 S127\n";
      }
      if (layer == 3) {
        file << "M106 S255\n";
      }

      file << ";PERIMETER\n";
      for (const auto &l : s.perimeter) {
        // Move to start of line
        file << "G0 F2700 X" << std::to_string(origin + l.p0.x) << " Y" << std::to_string(origin + l.p0.y) << " Z" << std::to_string(l.p0.z) << '\n';
        // Move to end while extruding
        auto dist = std::sqrt(std::pow(l.p1.x - l.p0.x, 2) + std::pow(l.p1.y - l.p0.y, 2));
        E += dist * extrudeRate;
        file << "G1 F900 X" << std::to_string(origin + l.p1.x) << " Y" << std::to_string(origin + l.p1.y) << " E" << std::to_string(E) << '\n';
      }

      // if (!s.support.empty())
      //   file << ";SUPPORT\n";
      
      // for (const auto &l : s.support) {
      //   // Move to start of line
      //   file << "G0 F2700 X" << std::to_string(origin + l.p0.x) << " Y" << std::to_string(origin + l.p0.y) << " Z" << std::to_string(l.p0.z) << '\n';
      //   // Move to end while extruding
      //   auto dist = std::sqrt(std::pow(l.p1.x - l.p0.x, 2) + std::pow(l.p1.y - l.p0.y, 2));
      //   E += dist * extrudeRate;
      //   file << "G1 F800 X" << std::to_string(origin + l.p1.x) << " Y" << std::to_string(origin + l.p1.y) << " E" << std::to_string(E) << '\n';
      // }

      if (!s.infill.empty())
        file << ";INFILL\n";

      for (const auto &l : s.infill) {
        // Move to start of line
        file << "G0 F2700 X" << std::to_string(origin + l.p0.x) << " Y" << std::to_string(origin + l.p0.y) << " Z" << std::to_string(l.p0.z) << '\n';
        // Move to end while extruding
        auto dist = std::sqrt(std::pow(l.p1.x - l.p0.x, 2) + std::pow(l.p1.y - l.p0.y, 2));
        E += dist * extrudeRate;
        file << "G1 F900 X" << std::to_string(origin + l.p1.x) << " Y" << std::to_string(origin + l.p1.y) << " E" << std::to_string(E) << '\n';
      }

      layer++;
    }

    // Postample
    file << ";END GCODE\n";
    file << "M104 S0\n";
    file << "M140 S0\n";
    file << "G91\n";
    file << "G1 E-1 F300\n";
    file << "G1 Z+0.5 E-5 X-20 Y-20 F2v700\n";
    file << "G28 X0 Y0\n";
    file << "M84\n";
    file << "G90\n";
  };
} // namespace qslicer