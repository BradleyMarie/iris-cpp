#include "iris/integrators/internal/sample_direct_lighting.h"

#include <cassert>
#include <cmath>

namespace iris {
namespace integrators {
namespace internal {
namespace internal {

visual_t PowerHeuristic(visual_t sampled_pdf, visual_t computed_pdf) {
  assert(std::isfinite(sampled_pdf) && 0.0 <= sampled_pdf);
  assert(std::isfinite(computed_pdf) && 0.0 <= computed_pdf);

  sampled_pdf *= sampled_pdf;
  computed_pdf *= computed_pdf;

  return sampled_pdf / (sampled_pdf + computed_pdf);
}

const Spectrum* DeltaLight(const Light::SampleResult& sample,
                           const Ray& traced_ray, const RayTracer::Result hit,
                           SpectralAllocator& allocator) {
  auto diffuse =
      hit.bsdf->Reflectance(traced_ray.direction, sample.to_light, allocator);
  if (!diffuse) {
    return nullptr;
  }

  visual_t falloff = AbsDotProduct(hit.shading_normal, sample.to_light);
  auto* spectrum = allocator.Scale(&sample.emission, falloff);
  return allocator.Reflect(spectrum, &diffuse->reflector);
}

const Spectrum* FromLightSample(const Light::SampleResult& sample,
                                const Ray& traced_ray,
                                const RayTracer::Result hit,
                                VisibilityTester& visibility_tester,
                                SpectralAllocator& allocator) {
  auto diffuse =
      hit.bsdf->Reflectance(traced_ray.direction, sample.to_light, allocator);
  if (!diffuse) {
    return nullptr;
  }

  visual_t falloff = AbsDotProduct(hit.shading_normal, sample.to_light);
  visual_t weight = PowerHeuristic(*sample.pdf, diffuse->pdf);
  visual_t attenuation = (falloff * weight) / *sample.pdf;
  auto* spectrum = allocator.Scale(&sample.emission, attenuation);
  return allocator.Reflect(spectrum, &diffuse->reflector);
}

const Spectrum* FromBsdfSample(const Bsdf::SampleResult& sample,
                               const Light& light, const Ray& traced_ray,
                               const RayTracer::Result hit,
                               VisibilityTester& visibility_tester,
                               SpectralAllocator& allocator) {
  assert(sample.pdf);  // Perfectly specular BSDFs should not use this path

  visual_t light_pdf;
  auto emissions = light.Emission(Ray(hit.hit_point, sample.direction),
                                  visibility_tester, allocator, &light_pdf);
  if (!emissions || light_pdf <= 0.0) {
    return nullptr;
  }

  visual_t falloff = AbsDotProduct(hit.shading_normal, sample.direction);
  visual_t weight = PowerHeuristic(*sample.pdf, light_pdf);
  visual_t attenuation = (falloff * weight) / *sample.pdf;
  auto* spectrum = allocator.Scale(emissions, attenuation);
  return allocator.Reflect(spectrum, &sample.reflector);
}

}  // namespace internal

const Spectrum* EstimateDirectLighting(
    const Light& light, const Ray& traced_ray, const RayTracer::Result hit,
    Sampler bsdf_sampler, Sampler light_sampler,
    VisibilityTester& visibility_tester, SpectralAllocator& allocator) {
  assert(hit.bsdf);  // Caller must check that hit contained a BSDF

  auto bsdf_sample = hit.bsdf->Sample(traced_ray.direction,
                                      std::move(bsdf_sampler), allocator);

  auto light_sample = light.Sample(hit.hit_point, std::move(light_sampler),
                                   visibility_tester, allocator);

  if (light_sample && !light_sample->pdf) {
    return internal::DeltaLight(*light_sample, traced_ray, hit, allocator);
  }

  const Spectrum* light_spectrum = nullptr;
  if (light_sample) {
    light_spectrum = internal::FromLightSample(*light_sample, traced_ray, hit,
                                               visibility_tester, allocator);
  }

  const Spectrum* bsdf_spectrum = nullptr;
  if (bsdf_sample) {
    bsdf_spectrum = internal::FromBsdfSample(*bsdf_sample, light, traced_ray,
                                             hit, visibility_tester, allocator);
  }

  return allocator.Add(light_spectrum, bsdf_spectrum);
}

const Spectrum* SampleDirectLighting(LightSampler& light_sampler,
                                     const Ray& traced_ray,
                                     const RayTracer::Result hit, Random& rng,
                                     VisibilityTester& visibility_tester,
                                     SpectralAllocator& allocator) {
  assert(hit.bsdf);  // Caller must check that hit contained a BSDF

  const Spectrum* result = nullptr;
  for (auto* light_samples = light_sampler.Sample(hit.hit_point); light_samples;
       light_samples = light_samples->next) {
    Sampler bsdf_sampler(rng);
    Sampler light_sampler(rng);

    if (light_samples->pdf && *light_samples->pdf <= 0.0) {
      continue;
    }

    auto* direct_light = EstimateDirectLighting(
        light_samples->light, traced_ray, hit, std::move(bsdf_sampler),
        std::move(light_sampler), visibility_tester, allocator);

    if (light_samples->pdf) {
      direct_light = allocator.Scale(direct_light, 1.0 / *light_samples->pdf);
    }

    allocator.Add(result, direct_light);
  }
  return result;
}

}  // namespace internal
}  // namespace integrators
}  // namespace iris