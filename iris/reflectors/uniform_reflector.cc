#include "iris/reflectors/uniform_reflector.h"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace iris::reflectors {

UniformReflector::UniformReflector(visual reflectance)
    : reflectance_(reflectance) {
  assert(std::isfinite(reflectance));
  assert(reflectance >= 0.0);
  assert(reflectance <= 1.0);
}

visual_t UniformReflector::Reflectance(visual_t wavelength) const {
  return reflectance_;
}

visual_t UniformReflector::Albedo() const { return reflectance_; }

}  // namespace iris::reflectors