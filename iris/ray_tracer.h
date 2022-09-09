#ifndef _IRIS_RAY_TRACER_
#define _IRIS_RAY_TRACER_

#include <optional>

#include "iris/bsdf.h"
#include "iris/float.h"
#include "iris/ray.h"
#include "iris/scene.h"
#include "iris/spectrum.h"

namespace iris {
namespace internal {
class RayTracer;
}  // namespace internal

class RayTracer final {
 public:
  struct Result {
    const Bsdf* bsdf;
    const Spectrum* emission;
    const Point hit_point;
    const Vector surface_normal;
    const Vector shading_normal;
  };

  RayTracer(const Scene& scene, geometric minimum_distance,
            internal::RayTracer& ray_tracer)
      : scene_(scene),
        minimum_distance_(minimum_distance),
        ray_tracer_(ray_tracer) {}

  std::optional<Result> Trace(const Ray& ray);

 private:
  RayTracer(const RayTracer&) = delete;
  RayTracer& operator=(const RayTracer&) = delete;

  const Scene& scene_;
  geometric minimum_distance_;
  internal::RayTracer& ray_tracer_;
};

}  // namespace iris

#endif  // _IRIS_RAY_TRACER_