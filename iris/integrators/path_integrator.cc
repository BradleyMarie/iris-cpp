#include "iris/integrators/path_integrator.h"

#include <cassert>
#include <cmath>

#include "iris/integrators/internal/path_builder.h"
#include "iris/integrators/internal/russian_roulette.h"
#include "iris/integrators/internal/sample_direct_lighting.h"

namespace iris {
namespace integrators {

PathIntegrator::PathIntegrator(visual maximum_path_continue_probability,
                               visual path_continue_probability_cutoff,
                               uint8_t min_bounces,
                               uint8_t max_bounces) noexcept
    : maximum_path_continue_probability_(
          std::max(static_cast<visual>(0.0),
                   std::min(static_cast<visual>(1.0),
                            maximum_path_continue_probability))),
      path_continue_probability_cutoff_(
          std::max(static_cast<visual>(0.0),
                   std::min(static_cast<visual>(1.0),
                            path_continue_probability_cutoff))),
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
                                             path_continue_probability_cutoff_);
  internal::PathBuilder path_builder(reflectors_, spectra_, attenuations_);

  visual_t path_throughput = 1.0;
  bool add_light_emissions = true;
  Ray trace_ray = ray;
  for (uint8_t bounces = 0;; bounces++) {
    auto trace_result = ray_tracer.Trace(trace_ray);
    if (!trace_result) {
      break;
    }

    if (add_light_emissions) {
      path_builder.Add(trace_result->emission, spectral_allocator);
      add_light_emissions = false;
    }

    if (!trace_result->bsdf) {
      break;
    }

    for (auto* light_samples = light_sampler.Sample(trace_result->hit_point);
         light_samples; light_samples = light_samples->next) {
      if (light_samples->pdf && *light_samples->pdf <= 0.0) {
        continue;
      }

      auto* direct_light = internal::SampleDirectLighting(
          light_samples->light, trace_ray, *trace_result, rng,
          visibility_tester, spectral_allocator);

      if (light_samples->pdf) {
        direct_light =
            spectral_allocator.Scale(direct_light, 1.0 / *light_samples->pdf);
      }

      path_builder.Add(direct_light, spectral_allocator);
    }

    if (bounces == max_bounces_) {
      break;
    }

    auto bsdf_sample = trace_result->bsdf->Sample(trace_ray.direction, rng,
                                                  spectral_allocator);

    if (!bsdf_sample) {
      break;
    }

    path_throughput *= bsdf_sample->reflector.Albedo();

    visual_t attenuation;
    if (bsdf_sample->pdf) {
      attenuation =
          AbsDotProduct(trace_result->shading_normal, bsdf_sample->direction) /
          *bsdf_sample->pdf;
      path_throughput *= attenuation;
    } else {
      attenuation = 1.0;
      add_light_emissions = true;
    }

    if (min_bounces_ < bounces) {
      auto roulette_pdf = russian_roulette.Evaluate(rng, path_throughput);
      if (!roulette_pdf) {
        break;
      }

      visual_t inverse_roulette_pdf = 1.0 / *roulette_pdf;
      attenuation *= inverse_roulette_pdf;
      path_throughput *= inverse_roulette_pdf;
    }

    path_builder.Bounce(&bsdf_sample->reflector, attenuation);
    new (&trace_ray) Ray(trace_result->hit_point, bsdf_sample->direction);
  }

  return path_builder.Build(spectral_allocator);
}

std::unique_ptr<Integrator> PathIntegrator::Duplicate() const {
  return std::make_unique<PathIntegrator>(maximum_path_continue_probability_,
                                          path_continue_probability_cutoff_,
                                          min_bounces_, max_bounces_);
}

}  // namespace integrators
}  // namespace iris