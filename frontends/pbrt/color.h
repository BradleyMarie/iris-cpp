#ifndef _FRONTENDS_PBRT_COLOR_
#define _FRONTENDS_PBRT_COLOR_

#include <array>
#include <cassert>

#include "iris/float.h"

namespace iris::pbrt_frontend {

struct Color {
  enum Space { XYZ, RGB };

  Color(const std::array<visual_t, 3>& values, Space space)
      : values(values), space(space) {
    assert(std::isfinite(values[0]) && values[0] >= 0.0);
    assert(std::isfinite(values[1]) && values[1] >= 0.0);
    assert(std::isfinite(values[2]) && values[2] >= 0.0);
  }

  const std::array<visual_t, 3> values;
  const Space space;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_COLOR_