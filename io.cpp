#include "io.hpp"

#include "Point.hpp"

#include "stl_reader/stl_reader.h"

namespace qslicer {

  std::vector<Triangle> fileToTriangles(const std::string &fileName) {
    stl_reader::StlMesh <float, unsigned int> mesh (fileName);
    std::vector<Triangle> triangles;
    for (std::size_t i = 0; i < mesh.num_tris(); ++i) {
      const float *n = mesh.tri_normal(i);
      Point normal (n[0], n[1], n[2]);

      const float *p0_ = mesh.vrt_coords(mesh.tri_corner_ind(i, 0));
      Point p0 (p0_[0], p0_[1], p0_[2]);
      const float *p1_ = mesh.vrt_coords(mesh.tri_corner_ind(i, 1));
      Point p1 (p1_[0], p1_[1], p1_[2]);
      const float *p2_ = mesh.vrt_coords(mesh.tri_corner_ind(i, 2));
      Point p2 (p2_[0], p2_[1], p2_[2]);

      triangles.push_back(Triangle(p0, p1, p2, normal));
    }

    return triangles;
  }
} // namespace qslicer