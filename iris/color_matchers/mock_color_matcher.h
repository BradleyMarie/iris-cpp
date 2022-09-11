#ifndef _IRIS_COLOR_MATCHERS_MOCK_COLOR_MATCHER_
#define _IRIS_COLOR_MATCHERS_MOCK_COLOR_MATCHER_

#include "googlemock/include/gmock/gmock.h"
#include "iris/color_matcher.h"
#include "iris/float.h"
#include "iris/spectrum.h"

namespace iris {
namespace color_matchers {

class MockColorMatcher final : public ColorMatcher {
 public:
  MOCK_METHOD((std::array<visual_t, 3>), Match, (const Spectrum&),
              (const override));
  MOCK_METHOD(Color::Space, ColorSpace, (), (const override));
};

}  // namespace color_matchers
}  // namespace iris

#endif  // _IRIS_COLOR_MATCHERS_MOCK_COLOR_MATCHER_