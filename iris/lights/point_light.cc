#include "iris/lights/point_light.h"

#include <limits>

namespace iris {
namespace lights {

std::optional<Light::SampleResult> PointLight::Sample(
    const Point& hit_point, Random& rng, VisibilityTester& tester,
    SpectralAllocator& allocator) const {
  auto to_light = location_ - hit_point;
  if (!tester.Visible(Ray(hit_point, to_light), 1.0)) {
    return std::nullopt;
  }

  return Light::SampleResult{*spectrum_, Normalize(to_light),
                             std::numeric_limits<visual>::infinity()};
}

const Spectrum* PointLight::Emission(const Ray& to_light,
                                     VisibilityTester& tester,
                                     SpectralAllocator& allocator,
                                     visual* pdf) const {
  return nullptr;
}

}  // namespace lights
}  // namespace iris