#ifndef _IRIS_TESTING_RAY_TRACER_
#define _IRIS_TESTING_RAY_TRACER_

#include <functional>

#include "iris/environmental_light.h"
#include "iris/ray_tracer.h"

namespace iris {
namespace testing {

RayTracer& GetNoHitsRayTracer();

void ScopedNoHitsRayTracer(const EnvironmentalLight& environmental_light,
                           std::function<void(RayTracer&)> callback);

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_RAY_TRACER_