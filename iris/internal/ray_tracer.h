#ifndef _IRIS_INTERNAL_RAY_TRACER_
#define _IRIS_INTERNAL_RAY_TRACER_

#include "iris/float.h"
#include "iris/internal/hit.h"
#include "iris/internal/hit_arena.h"
#include "iris/ray.h"
#include "iris/scene.h"

namespace iris {
namespace internal {

struct RayTracer final {
 public:
  RayTracer() = default;

  Hit* Trace(const Ray& ray, geometric minimum_distance,
             geometric maximum_distance, const Scene& scene);

 private:
  RayTracer(const RayTracer&) = delete;
  RayTracer& operator=(const RayTracer&) = delete;

  HitArena hit_arena_;
};

}  // namespace internal
}  // namespace iris

#endif  // _IRIS_RAY_TRACER_