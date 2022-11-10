#ifndef _IRIS_ENVIRONMENTAL_LIGHTS_MOCK_ENVIRONMENTAL_LIGHT_
#define _IRIS_ENVIRONMENTAL_LIGHTS_MOCK_ENVIRONMENTAL_LIGHT_

#include "googlemock/include/gmock/gmock.h"
#include "iris/environmental_light.h"
#include "iris/float.h"
#include "iris/sampler.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/vector.h"

namespace iris {
namespace environmental_lights {

class MockEnvironmentalLight final : public EnvironmentalLight {
 public:
  MOCK_METHOD(std::optional<SampleResult>, Sample,
              (Sampler&, SpectralAllocator&), (const override));
  MOCK_METHOD(const Spectrum*, Emission,
              (const Vector&, SpectralAllocator&, visual_t*), (const override));
};

}  // namespace environmental_lights
}  // namespace iris

#endif  // _IRIS_ENVIRONMENTAL_LIGHTS_MOCK_ENVIRONMENTAL_LIGHT_