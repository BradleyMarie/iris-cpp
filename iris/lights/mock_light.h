#ifndef _IRIS_LIGHTS_MOCK_LIGHT_
#define _IRIS_LIGHTS_MOCK_LIGHT_

#include <optional>

#include "googlemock/include/gmock/gmock.h"
#include "iris/float.h"
#include "iris/light.h"
#include "iris/point.h"
#include "iris/ray.h"
#include "iris/sampler.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace lights {

class MockLight final : public Light {
 public:
  MOCK_METHOD((std::optional<SampleResult>), Sample,
              (const Point&, Sampler, VisibilityTester&, SpectralAllocator&),
              (const override));

  MOCK_METHOD(const Spectrum*, Emission,
              (const Ray&, VisibilityTester&, SpectralAllocator&, visual_t*),
              (const override));
};

}  // namespace lights
}  // namespace iris

#endif  // _IRIS_LIGHTS_MOCK_LIGHT_