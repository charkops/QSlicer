#include "Slice.hpp"

namespace qslicer {
  Slice cleanPerimeter(const Slice &s) {
    auto setPerimeter = s.perimeter;

    std::vector<Line>::size_type i = 0;
    while (i < setPerimeter.size()) {
      auto j = i + 1;
      while (j < setPerimeter.size()) {
        if (lineEqual(setPerimeter[i], setPerimeter[j])) {
          // TODO (@charkops): Does this really do what i think it does ?
          setPerimeter.erase(setPerimeter.begin() + j);
        } else {
          j++;
        }
      }
      i++;
    }
    return Slice(s.zvalue, setPerimeter, s.isSurface);
  };
} // namespace qslicer