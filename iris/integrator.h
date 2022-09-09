#ifndef _IRIS_INTEGRATOR_
#define _IRIS_INTEGRATOR_

#include "iris/light_sampler.h"
#include "iris/random.h"
#include "iris/ray.h"
#include "iris/ray_tracer.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/visibility_tester.h"

namespace iris {

class Integrator {
 public:
  virtual const Spectrum* Integrate(const Ray& ray,
                                    const LightSampler& light_sampler,
                                    RayTracer& ray_tracer,
                                    VisibilityTester& visibility_tester,
                                    SpectralAllocator& spectral_allocator,
                                    Random& rng) const = 0;
  virtual ~Integrator() {}
};

}  // namespace iris

#endif  // _IRIS_INTEGRATOR_