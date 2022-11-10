#ifndef _IRIS_INTERNAL_ENVIRONMENTAL_LIGHT_
#define _IRIS_INTERNAL_ENVIRONMENTAL_LIGHT_

#include <optional>

#include "iris/environmental_light.h"
#include "iris/float.h"
#include "iris/geometry.h"
#include "iris/integer.h"
#include "iris/light.h"
#include "iris/matrix.h"
#include "iris/ray.h"
#include "iris/reference_counted.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/visibility_tester.h"

namespace iris::internal {

class EnvironmentalLight final : public Light {
 public:
  EnvironmentalLight(
      iris::ReferenceCounted<iris::EnvironmentalLight> light) noexcept
      : light_(std::move(light)) {}

  std::optional<SampleResult> Sample(
      const Point& hit_point, Sampler sampler, iris::VisibilityTester& tester,
      SpectralAllocator& allocator) const override;

  const Spectrum* Emission(const Ray& to_light, iris::VisibilityTester& tester,
                           SpectralAllocator& allocator,
                           visual_t* pdf = nullptr) const override;

 private:
  const iris::ReferenceCounted<iris::EnvironmentalLight> light_;
};

}  // namespace iris::internal

#endif  // _IRIS_INTERNAL_ENVIRONMENTAL_LIGHT_