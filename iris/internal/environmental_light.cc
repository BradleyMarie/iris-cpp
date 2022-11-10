#include "iris/internal/environmental_light.h"

namespace iris::internal {

std::optional<Light::SampleResult> EnvironmentalLight::Sample(
    const Point& hit_point, Sampler sampler, iris::VisibilityTester& tester,
    SpectralAllocator& allocator) const {
  auto sample = light_->Sample(sampler, allocator);
  if (!sample) {
    return std::nullopt;
  }

  if (!tester.Visible(Ray(hit_point, sample->to_light))) {
    return std::nullopt;
  }

  return Light::SampleResult{sample->emission, sample->to_light, sample->pdf};
}

const Spectrum* EnvironmentalLight::Emission(const Ray& to_light,
                                             iris::VisibilityTester& tester,
                                             SpectralAllocator& allocator,
                                             visual_t* pdf) const {
  auto* emissions = light_->Emission(to_light.direction, allocator, pdf);
  if (!emissions) {
    return nullptr;
  }

  if (!tester.Visible(to_light)) {
    return nullptr;
  }

  return emissions;
}

}  // namespace iris::internal