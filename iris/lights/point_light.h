#ifndef _IRIS_LIGHTS_POINT_LIGHT_
#define _IRIS_LIGHTS_POINT_LIGHT_

#include <memory>
#include <optional>

#include "iris/float.h"
#include "iris/light.h"
#include "iris/point.h"
#include "iris/random.h"
#include "iris/ray.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace lights {

class PointLight final : public Light {
 public:
  PointLight(std::shared_ptr<Spectrum> spectrum, Point location) noexcept
      : spectrum_(std::move(spectrum)), location_(location) {}

  std::optional<SampleResult> Sample(
      const Point& hit_point, Random& rng, VisibilityTester& tester,
      SpectralAllocator& allocator) const override;

  const Spectrum* Emission(const Ray& to_light, VisibilityTester& tester,
                           SpectralAllocator& allocator,
                           visual_t* pdf) const override;

 private:
  std::shared_ptr<Spectrum> spectrum_;
  Point location_;
};

}  // namespace lights
}  // namespace iris

#endif  // _IRIS_LIGHTS_POINT_LIGHT_