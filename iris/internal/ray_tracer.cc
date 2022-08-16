#include "iris/internal/ray_tracer.h"

namespace iris {
namespace internal {

Hit* RayTracer::Trace(const Ray& ray, geometric_t minimum_distance,
                      geometric_t maximum_distance, const Scene& scene) {
  hit_arena_.Clear();

  iris::Hit* closest_hit;
  Intersector intersector(ray, minimum_distance, maximum_distance, hit_arena_,
                          &closest_hit);
  scene.Trace(ray, intersector);

  return static_cast<Hit*>(closest_hit);
}

}  // namespace internal
}  // namespace iris