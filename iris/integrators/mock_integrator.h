#ifndef _IRIS_INTEGRATORS_MOCK_INTEGRATOR_
#define _IRIS_INTEGRATORS_MOCK_INTEGRATOR_

#include <memory>

#include "googlemock/include/gmock/gmock.h"
#include "iris/integrator.h"
#include "iris/light_sampler.h"
#include "iris/random.h"
#include "iris/ray.h"
#include "iris/ray_tracer.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace integrators {

class MockIntegrator final : public Integrator {
 public:
  MOCK_METHOD(const Spectrum*, Integrate,
              (const Ray&, RayTracer&, LightSampler&, VisibilityTester&,
               SpectralAllocator&, Random&),
              (override));
  MOCK_METHOD(std::unique_ptr<Integrator>, Duplicate, (), (const override));
};

}  // namespace integrators
}  // namespace iris

#endif  // _IRIS_INTEGRATORS_MOCK_INTEGRATOR_