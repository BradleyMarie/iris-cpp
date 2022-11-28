#include "frontends/pbrt/integrators/pixel_bounds.h"

#include <cstdlib>
#include <iostream>

namespace iris::pbrt_frontend::integrators {

std::array<size_t, 4> ParsePixelBounds(const Parameter& parameter) {
  auto values = parameter.GetIntegerValues(4, 4);
  if (values[0] < 0 || values[1] < 0 || values[2] < 0 || values[3] < 0) {
    std::cerr << "ERROR: Negative value in parameter list: pixelbounds"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto x_min = static_cast<uint64_t>(values[0]);
  auto x_max = static_cast<uint64_t>(values[1]);
  auto y_min = static_cast<uint64_t>(values[2]);
  auto y_max = static_cast<uint64_t>(values[3]);

  if (x_min > std::numeric_limits<size_t>::max() ||
      x_max > std::numeric_limits<size_t>::max() ||
      y_min > std::numeric_limits<size_t>::max() ||
      y_max > std::numeric_limits<size_t>::max()) {
    std::cerr << "ERROR: Out of range value in parameter list: pixelbounds"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (x_max < x_min || y_max < y_min) {
    std::cerr << "ERROR: Invalid values for parameter list: pixelbounds"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::array<size_t, 4>({x_min, x_max, y_min, y_max});
}

}  // namespace iris::pbrt_frontend::integrators