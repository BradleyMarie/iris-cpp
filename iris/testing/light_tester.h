#ifndef _IRIS_TESTING_LIGHT_TESTER_
#define _IRIS_TESTING_LIGHT_TESTER_

#include <optional>

#include "iris/float.h"
#include "iris/internal/arena.h"
#include "iris/light.h"
#include "iris/point.h"
#include "iris/random.h"
#include "iris/scene.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace testing {

class LightTester {
 public:
  std::optional<iris::Light::SampleResult> Sample(const Light& light,
                                                  const Point& hit_point,
                                                  Random& rng,
                                                  const Scene& scene);

  const Spectrum* Emission(const Light& light, const Ray& to_light,
                           const Scene& scene, visual_t* pdf = nullptr);

 private:
  internal::Arena arena_;
};

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_LIGHT_TESTER_