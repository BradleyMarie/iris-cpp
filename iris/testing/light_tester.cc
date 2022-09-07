#include "iris/testing/light_tester.h"

#include "iris/internal/ray_tracer.h"
#include "iris/internal/visibility_tester.h"
#include "iris/spectral_allocator.h"

namespace iris {
namespace testing {

std::optional<iris::Light::SampleResult> LightTester::Sample(
    const Light& light, const Point& hit_point, Random& rng,
    MockVisibilityTester& visibility_tester) const {
  internal::RayTracer ray_tracer;
  internal::VisibilityTester real_visibility_tester(
      visibility_tester, static_cast<geometric_t>(0.0), ray_tracer);
  SpectralAllocator allocator(arena_);
  return light.Sample(hit_point, rng, real_visibility_tester, allocator);
}

const Spectrum* LightTester::Emission(const Light& light, const Ray& to_light,
                                      MockVisibilityTester& visibility_tester,
                                      visual_t* pdf) const {
  internal::RayTracer ray_tracer;
  internal::VisibilityTester real_visibility_tester(
      visibility_tester, static_cast<geometric_t>(0.0), ray_tracer);
  SpectralAllocator allocator(arena_);
  return light.Emission(to_light, real_visibility_tester, allocator, pdf);
};

}  // namespace testing
}  // namespace iris