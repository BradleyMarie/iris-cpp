#ifndef _IRIS_COLOR_MATCHERS_CIE_COLOR_MATCHER_
#define _IRIS_COLOR_MATCHERS_CIE_COLOR_MATCHER_

#include <array>

#include "iris/color.h"
#include "iris/color_matcher.h"
#include "iris/float.h"
#include "iris/spectrum.h"

namespace iris {
namespace color_matchers {

class CieColorMatcher final : public ColorMatcher {
 public:
  std::array<visual_t, 3> Match(const Spectrum& spectrum) const override;
  Color::Space ColorSpace() const override;
};

}  // namespace color_matchers
}  // namespace iris

#endif  // _IRIS_COLOR_MATCHERS_CIE_COLOR_MATCHER_