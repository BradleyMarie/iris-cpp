#ifndef _IRIS_TESTING_LIGHT_TESTER_
#define _IRIS_TESTING_LIGHT_TESTER_

#include <optional>

#include "iris/float.h"
#include "iris/internal/arena.h"
#include "iris/light.h"
#include "iris/point.h"
#include "iris/random.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/testing/mock_visibility_tester.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace testing {

class LightTester {
 public:
  std::optional<iris::Light::SampleResult> Sample(
      const Light& light, const Point& hit_point, Random& rng,
      MockVisibilityTester& visibility_tester) const;

  const Spectrum* Emission(const Light& light, const Ray& to_light,
                           MockVisibilityTester& visibility_tester,
                           visual_t* pdf = nullptr) const;

 private:
  mutable internal::Arena arena_;
};

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_LIGHT_TESTER_