#ifndef _IRIS_INTEGRATORS_MOCK_INTEGRATOR_
#define _IRIS_INTEGRATORS_MOCK_INTEGRATOR_

#include <memory>
#include <vector>

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

class PathIntegrator final : public Integrator {
 public:
  PathIntegrator(visual maximum_path_continue_probability,
                 visual path_continue_probability_cutoff, uint8_t min_bounces,
                 uint8_t max_bounces) noexcept;

  virtual const Spectrum* Integrate(const Ray& ray, RayTracer& ray_tracer,
                                    LightSampler& light_sampler,
                                    VisibilityTester& visibility_tester,
                                    SpectralAllocator& spectral_allocator,
                                    Random& rng) override;

  virtual std::unique_ptr<Integrator> Duplicate() const override;

 private:
  std::vector<const Reflector*> reflectors_;
  std::vector<const Spectrum*> spectra_;
  std::vector<visual_t> attenuations_;
  visual maximum_path_continue_probability_;
  visual path_continue_probability_cutoff_;
  uint8_t min_bounces_;
  uint8_t max_bounces_;
};

}  // namespace integrators
}  // namespace iris

#endif  // _IRIS_INTEGRATORS_MOCK_INTEGRATOR_