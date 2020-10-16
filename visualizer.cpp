#include <iostream>
#include <algorithm>

#include "Point.hpp"
#include "Line.hpp"
#include "Triangle.hpp"
#include "io.hpp"
#include "Writer.hpp"

#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

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

void addLineToViewer(const pcl::PointXYZ &p0, const pcl::PointXYZ &p1, pcl::visualization::PCLVisualizer::Ptr viewer) {
  static long int counter = 1;
  std::stringstream ss;
  ss << "line_" << counter++;
  viewer->addLine(p0, p1, ss.str());
};

void addTriangleToViewer(const Triangle &tri, pcl::visualization::PCLVisualizer::Ptr viewer) {
  pcl::PointXYZ p0 (tri.p0.x, tri.p0.y, tri.p0.z);
  pcl::PointXYZ p1 (tri.p1.x, tri.p1.y, tri.p1.z);
  pcl::PointXYZ p2 (tri.p2.x, tri.p2.y, tri.p2.z);

  addLineToViewer(p0, p1, viewer);
  addLineToViewer(p1, p2, viewer);
  addLineToViewer(p2, p0, viewer);
};

struct ViewerOptions {
  bool addInfill = false;
  bool addSupport = false;
};

void addSliceToViewer(const Slice &slice, pcl::visualization::PCLVisualizer::Ptr viewer, const ViewerOptions &options) {
  // Add perimeter
  // Draw each line in perimeter
  for (const auto &line : slice.perimeter) {
    pcl::PointXYZ p0 (line.p0.x, line.p0.y, line.p0.z);
    pcl::PointXYZ p1 (line.p1.x, line.p1.y, line.p1.z);
    
    addLineToViewer(p0, p1, viewer);
  }

  // Add infill
  if (options.addInfill) {
    for (const auto &line : slice.infill) {
      pcl::PointXYZ p0 (line.p0.x, line.p0.y, line.p0.z);
      pcl::PointXYZ p1 (line.p1.x, line.p1.y, line.p1.z);
    
      addLineToViewer(p0, p1, viewer);
    }
  }

  if (options.addSupport) {
    for (const auto &outer : slice.support) {
      for (const auto &line : outer) {
        pcl::PointXYZ p0 (line.p0.x, line.p0.y, line.p0.z);
        pcl::PointXYZ p1 (line.p1.x, line.p1.y, line.p1.z);
    
        addLineToViewer(p0, p1, viewer);
      }
    }
  }
};

struct ViewerSlices {
  pcl::visualization::PCLVisualizer::Ptr viewer;
  std::vector<Slice> slices;
  ViewerOptions options = {true, true};
};

void keyboardEventOccured(const pcl::visualization::KeyboardEvent &event, void *viewer_slices_void) {  
  if (event.keyDown()) {
    auto viewerSlices = *static_cast<ViewerSlices *>(viewer_slices_void);
    static auto idx = viewerSlices.slices.size() / 2;

    if (event.getKeySym() == "n" || event.getKeySym() == "Up") {
      if (idx + 1 <= viewerSlices.slices.size() - 1)
        idx++;
    }
    if (event.getKeySym() == "p" || event.getKeySym() == "Down") {
      if (idx >= 1)
        idx--;
    }
    cout << "[idx]: " << idx << endl;
    cout << "[perimeter.size()]: " << viewerSlices.slices[idx].perimeter.size() << endl;

    auto viewer = viewerSlices.viewer;
    viewer->removeAllShapes();
    addSliceToViewer(viewerSlices.slices[idx], viewerSlices.viewer, viewerSlices.options);
    // viewer->resetCamera();
  }
};

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Provide an .stl file\n";
    return EXIT_FAILURE;
  }

  const std::string fileName (argv[1]);
  const float layerThickness = 0.4f;
  const float infillPercent = 0.02f;
  const float supportInfillPercentage = 0.02f;

  
  auto triangles = fileToTriangles(fileName);
  cout << "Found " << triangles.size() << " triangles\n";

  auto slices = separateSlices(triangles, layerThickness);

  
  for (auto &s : slices) {
    s = cleanPerimeter(s);
    if (s.isSurface)
      s.infill = infill(s.perimeter, 0.05);
    else
      s.infill = infill(s.perimeter, infillPercent);
  }
  
  auto supportSlices = generateSupports(triangles, layerThickness);
  
  for (const auto &shape : supportSlices) {
    for (std::vector<Slice>::size_type s = 0; s < shape.size(); s++) {
      slices[s].support.push_back(infill(shape[s].perimeter, supportInfillPercentage));
    }
  }
  
  // writeGcode(slices, fileName);

  pcl::visualization::PCLVisualizer::Ptr viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
  ViewerSlices viewerSlices;
  viewerSlices.viewer = viewer;
  viewerSlices.slices = slices;
  viewerSlices.options = {false, true};
  viewer->registerKeyboardCallback(keyboardEventOccured, (void *) &viewerSlices);
  
  addSliceToViewer(slices[slices.size() / 2], viewer, viewerSlices.options);

  cout <<slices[slices.size() / 2].perimeter.size() << '\n';

  viewer->resetCamera();
  while(!viewer->wasStopped())
    viewer->spinOnce(30);

  return EXIT_SUCCESS;
}