#ifndef _IRIS_COLOR_MATCHER_
#define _IRIS_COLOR_MATCHER_

#include <array>

#include "iris/color.h"
#include "iris/spectrum.h"

namespace iris {

class ColorMatcher {
 public:
  virtual std::array<visual_t, 3> Match(const Spectrum& spectrum) const = 0;
  virtual Color::Space ColorSpace() const = 0;
  virtual ~ColorMatcher() {}
};

}  // namespace iris

#endif  // _IRIS_COLOR_MATCHER_