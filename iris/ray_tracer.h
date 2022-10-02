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
class Arena;
class RayTracer;
}  // namespace internal

class RayTracer final {
 public:
  RayTracer(const Scene& scene, geometric_t minimum_distance,
            internal::RayTracer& ray_tracer, internal::Arena& arena) noexcept
      : scene_(scene),
        minimum_distance_(minimum_distance),
        ray_tracer_(ray_tracer),
        arena_(arena) {}

  struct Result {
    std::optional<Bsdf> bsdf;
    const Spectrum* emission;
    Point hit_point;
    Vector surface_normal;
    Vector shading_normal;
  };

  std::optional<Result> Trace(const Ray& ray);

 private:
  RayTracer(const RayTracer&) = delete;
  RayTracer& operator=(const RayTracer&) = delete;

  const Scene& scene_;
  geometric_t minimum_distance_;
  internal::RayTracer& ray_tracer_;
  internal::Arena& arena_;
};

}  // namespace iris

#endif  // _IRIS_RAY_TRACER_