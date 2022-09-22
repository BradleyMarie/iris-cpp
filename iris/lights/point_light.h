#ifndef _IRIS_LIGHTS_POINT_LIGHT_
#define _IRIS_LIGHTS_POINT_LIGHT_

#include <cassert>
#include <memory>
#include <optional>

#include "iris/float.h"
#include "iris/light.h"
#include "iris/point.h"
#include "iris/random.h"
#include "iris/ray.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/utility/spectrum_manager.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace lights {

class PointLight final : public Light {
 public:
  PointLight(Point location,
             std::shared_ptr<iris::utility::SpectrumManager> spectrum_manager,
             size_t index) noexcept
      : spectrum_manager_(std::move(spectrum_manager)),
        spectrum_(*spectrum_manager_->Get(index)),
        location_(location) {
    assert(index != 0);
  }

  std::optional<SampleResult> Sample(
      const Point& hit_point, Random& rng, VisibilityTester& tester,
      SpectralAllocator& allocator) const override;

  const Spectrum* Emission(const Ray& to_light, VisibilityTester& tester,
                           SpectralAllocator& allocator,
                           visual_t* pdf) const override;

 private:
  const std::shared_ptr<iris::utility::SpectrumManager> spectrum_manager_;
  const Spectrum& spectrum_;
  const Point location_;
};

}  // namespace lights
}  // namespace iris

#endif  // _IRIS_LIGHTS_POINT_LIGHT_