#include "iris/integrators/path_integrator.h"

#include <cassert>
#include <cmath>

#include "iris/integrators/internal/path_builder.h"
#include "iris/integrators/internal/russian_roulette.h"
#include "iris/integrators/internal/sample_direct_lighting.h"

namespace iris {
namespace integrators {

PathIntegrator::PathIntegrator(visual maximum_path_continue_probability,
                               visual always_continue_threshold,
                               uint8_t min_bounces,
                               uint8_t max_bounces) noexcept
    : maximum_path_continue_probability_(
          std::max(static_cast<visual>(0.0),
                   std::min(static_cast<visual>(1.0),
                            maximum_path_continue_probability))),
      always_continue_threshold_(std::max(
          static_cast<visual>(0.0),
          std::min(static_cast<visual>(1.0), always_continue_threshold))),
      min_bounces_(min_bounces),
      max_bounces_(max_bounces) {
  reflectors_.reserve(max_bounces);
  spectra_.reserve(max_bounces);
  attenuations_.reserve(max_bounces);
}

const Spectrum* PathIntegrator::Integrate(const Ray& ray, RayTracer& ray_tracer,
                                          LightSampler& light_sampler,
                                          VisibilityTester& visibility_tester,
                                          SpectralAllocator& spectral_allocator,
                                          Random& rng) {
  internal::RussianRoulette russian_roulette(maximum_path_continue_probability_,
                                             always_continue_threshold_);
  internal::PathBuilder path_builder(reflectors_, spectra_, attenuations_);

  visual_t path_throughput = 1.0;
  bool add_light_emissions = true;
  Ray trace_ray = ray;
  for (uint8_t bounces = 0;; bounces++) {
    auto trace_result = ray_tracer.Trace(trace_ray);

    if (add_light_emissions) {
      path_builder.Add(trace_result.emission, spectral_allocator);
      add_light_emissions = false;
    }

    if (!trace_result.surface_intersection) {
      break;
    }

    auto* direct_lighting = internal::SampleDirectLighting(
        light_sampler, trace_ray, *trace_result.surface_intersection, rng,
        visibility_tester, spectral_allocator);
    path_builder.Add(direct_lighting, spectral_allocator);

    if (bounces == max_bounces_) {
      break;
    }

    auto bsdf_sample = trace_result.surface_intersection->bsdf.Sample(
        trace_ray.direction, iris::Sampler(rng), spectral_allocator);

    if (!bsdf_sample) {
      break;
    }

    path_throughput *= bsdf_sample->reflector.Albedo();

    visual_t attenuation;
    if (bsdf_sample->pdf) {
      attenuation =
          AbsDotProduct(trace_result.surface_intersection->shading_normal,
                        bsdf_sample->direction) /
          *bsdf_sample->pdf;
      path_throughput *= attenuation;
    } else {
      attenuation = 1.0;
      add_light_emissions = true;
    }

    if (min_bounces_ <= bounces) {
      auto roulette_pdf = russian_roulette.Evaluate(rng, path_throughput);
      if (!roulette_pdf) {
        break;
      }

      visual_t inverse_roulette_pdf =
          static_cast<visual_t>(1.0) / *roulette_pdf;
      attenuation *= inverse_roulette_pdf;
      path_throughput *= inverse_roulette_pdf;
    }

    path_builder.Bounce(&bsdf_sample->reflector, attenuation);
    new (&trace_ray) Ray(trace_result.surface_intersection->hit_point,
                         bsdf_sample->direction);
  }

  return path_builder.Build(spectral_allocator);
}

std::unique_ptr<Integrator> PathIntegrator::Duplicate() const {
  return std::make_unique<PathIntegrator>(maximum_path_continue_probability_,
                                          always_continue_threshold_,
                                          min_bounces_, max_bounces_);
}

}  // namespace integrators
}  // namespace iris