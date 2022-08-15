#include "iris/intersector.h"

#include "iris/hit_allocator.h"
#include "iris/internal/hit.h"

namespace iris {

void Intersector::Intersect(const Geometry& geometry) {
  HitAllocator allocator(ray_, hit_arena_);
  auto* hit_list = geometry.Trace(allocator);
  while (hit_list) {
    if (hit_list->distance > minimum_distance_ &&
        hit_list->distance < maximum_distance_) {
      *closest_hit_ = hit_list;
      maximum_distance_ = hit_list->distance;
    }
    hit_list = hit_list->next;
  }
}

void Intersector::Intersect(const Geometry& geometry,
                            const Matrix& model_to_world) {
  Ray trace_ray = model_to_world.InverseMultiply(ray_);
  HitAllocator allocator(trace_ray, hit_arena_);

  auto* hit_list = geometry.Trace(allocator);
  while (hit_list) {
    if (hit_list->distance > minimum_distance_ &&
        hit_list->distance < maximum_distance_) {
      auto* full_hit = static_cast<internal::Hit*>(hit_list);
      full_hit->model_to_world = &model_to_world;
      *closest_hit_ = hit_list;
      maximum_distance_ = hit_list->distance;
    }
    hit_list = hit_list->next;
  }
}

}  // namespace iris