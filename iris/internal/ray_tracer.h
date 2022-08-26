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
  Hit* Trace(const Ray& ray, geometric_t minimum_distance,
             geometric_t maximum_distance, const Scene& scene);

 private:
  HitArena hit_arena_;
};

}  // namespace internal
}  // namespace iris

#endif  // _IRIS_RAY_TRACER_