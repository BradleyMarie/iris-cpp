#ifndef _IRIS_INTEGRATORS_INTERNAL_SAMPLE_DIRECT_LIGHTING_
#define _IRIS_INTEGRATORS_INTERNAL_SAMPLE_DIRECT_LIGHTING_

#include "iris/light.h"
#include "iris/light_sampler.h"
#include "iris/random.h"
#include "iris/ray.h"
#include "iris/ray_tracer.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace integrators {
namespace internal {
namespace internal {

visual_t PowerHeuristic(visual_t sampled_pdf, visual_t computed_pdf);

const Spectrum* DeltaLight(const Light::SampleResult& sample,
                           const Ray& traced_ray, const RayTracer::Result hit,
                           SpectralAllocator& allocator);

const Spectrum* FromLightSample(const Light::SampleResult& sample,
                                const Ray& traced_ray,
                                const RayTracer::Result hit,
                                VisibilityTester& visibility_tester,
                                SpectralAllocator& allocator);

const Spectrum* FromBsdfSample(const Bsdf::SampleResult& sample,
                               const Light& light, const Ray& traced_ray,
                               const RayTracer::Result hit,
                               VisibilityTester& visibility_tester,
                               SpectralAllocator& allocator);

}  // namespace internal

const Spectrum* EstimateDirectLighting(const Light& light,
                                       const Ray& traced_ray,
                                       const RayTracer::Result hit, Random& rng,
                                       VisibilityTester& visibility_tester,
                                       SpectralAllocator& allocator);

const Spectrum* SampleDirectLighting(LightSampler& light_sampler,
                                     const Ray& traced_ray,
                                     const RayTracer::Result hit, Random& rng,
                                     VisibilityTester& visibility_tester,
                                     SpectralAllocator& allocator);

}  // namespace internal
}  // namespace integrators
}  // namespace iris

#endif  // _IRIS_INTEGRATORS_INTERNAL_SAMPLE_DIRECT_LIGHTING_