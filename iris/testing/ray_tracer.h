#ifndef _IRIS_TESTING_RAY_TRACER_
#define _IRIS_TESTING_RAY_TRACER_

#include <functional>
#include <span>

#include "iris/environmental_light.h"
#include "iris/ray_tracer.h"

namespace iris {
namespace testing {

RayTracer& GetNoHitsRayTracer();

void ScopedNoHitsRayTracer(const EnvironmentalLight& environmental_light,
                           std::function<void(RayTracer&)> callback);

struct RayTracerPathNode {
  const geometric_t distance;
  const Spectrum* emission;
  const Bxdf* bxdf;
  const Vector surface_normal;
  const Vector shading_normal;
};

void ScopedHitsRayTracer(const EnvironmentalLight* environmental_light,
                         std::span<const RayTracerPathNode> trace_results,
                         std::function<void(RayTracer&)> callback);

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_RAY_TRACER_