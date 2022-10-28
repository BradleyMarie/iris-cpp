#include "iris/integrators/path_integrator.h"

#include <cassert>
#include <cmath>

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

  visual_t path_throughput = 1.0;
  bool add_light_emissions = true;
  Ray trace_ray = ray;
  uint8_t bounces = 0u;
  for (;;) {
    auto trace_result = ray_tracer.Trace(trace_ray);
    if (!trace_result) {
      break;
    }

    if (add_light_emissions) {
      spectra_.push_back(trace_result->emission);
      add_light_emissions = false;
    } else {
      spectra_.push_back(nullptr);
    }

    if (!trace_result->bsdf) {
      break;
    }

    auto bsdf_sample = trace_result->bsdf->Sample(trace_ray.direction, rng,
                                                  spectral_allocator);

    // Only evaluate direct lighting if BSDF is not perfectly specular
    if (!bsdf_sample || bsdf_sample->pdf) {
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

        spectra_[bounces] =
            spectral_allocator.Add(spectra_[bounces], direct_light);
      }
    } else {
      add_light_emissions = true;
    }

    if (bounces == max_bounces_) {
      break;
    }

    if (!bsdf_sample) {
      break;
    }

    path_throughput *= bsdf_sample->reflector.Albedo();

    visual_t attenuation = 1.0;
    if (bsdf_sample->pdf) {
      attenuation =
          AbsDotProduct(trace_result->shading_normal, bsdf_sample->direction) /
          *bsdf_sample->pdf;
      path_throughput *= attenuation;
    }

    if (min_bounces_ < bounces) {
      auto roulette_pdf = russian_roulette.Evaluate(rng, path_throughput);
      if (!roulette_pdf) {
        break;
      }

      attenuation /= *roulette_pdf;
      path_throughput /= *roulette_pdf;
    }

    attenuations_.push_back(attenuation);
    reflectors_.push_back(&bsdf_sample->reflector);

    new (&trace_ray) Ray(trace_result->hit_point, bsdf_sample->direction);
    bounces += 1;
  }

  while (!reflectors_.empty()) {
    auto* spectra = spectra_.back();
    spectra_.pop_back();

    auto* reflector = reflectors_.back();
    reflectors_.pop_back();

    auto attenuation = attenuations_.back();
    attenuations_.pop_back();

    spectra = spectral_allocator.Scale(spectra, attenuation);
    spectra = spectral_allocator.Reflect(spectra, reflector);
    spectra_.back() = spectral_allocator.Add(spectra, spectra_.back());
  }

  return spectra_.back();
}

std::unique_ptr<Integrator> PathIntegrator::Duplicate() const {
  return std::make_unique<PathIntegrator>(maximum_path_continue_probability_,
                                          path_continue_probability_cutoff_,
                                          min_bounces_, max_bounces_);
}

}  // namespace integrators
}  // namespace iris