#ifndef _IRIS_LIGHTS_POINT_LIGHT_
#define _IRIS_LIGHTS_POINT_LIGHT_

#include <cassert>
#include <optional>

#include "iris/float.h"
#include "iris/light.h"
#include "iris/point.h"
#include "iris/random.h"
#include "iris/ray.h"
#include "iris/reference_counted.h"
#include "iris/spectra/reference_countable_spectrum.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace lights {

class PointLight final : public Light {
 public:
  PointLight(Point location,
             iris::ReferenceCounted<iris::spectra::ReferenceCountableSpectrum>
                 spectrum) noexcept
      : spectrum_(std::move(spectrum)), location_(location) {
    assert(spectrum_);
  }

  std::optional<SampleResult> Sample(
      const Point& hit_point, Random& rng, VisibilityTester& tester,
      SpectralAllocator& allocator) const override;

  const Spectrum* Emission(const Ray& to_light, VisibilityTester& tester,
                           SpectralAllocator& allocator,
                           visual_t* pdf) const override;

 private:
  const iris::ReferenceCounted<iris::spectra::ReferenceCountableSpectrum>
      spectrum_;
  const Point location_;
};

}  // namespace lights
}  // namespace iris

#endif  // _IRIS_LIGHTS_POINT_LIGHT_