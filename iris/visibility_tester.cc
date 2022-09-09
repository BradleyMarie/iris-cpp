#include "iris/visibility_tester.h"

#include "iris/internal/ray_tracer.h"

namespace iris {

bool VisibilityTester::Visible(const Ray& ray, geometric maximum_distance) {
  auto* hit =
      ray_tracer_.Trace(ray, minimum_distance_, maximum_distance, scene_);
  return hit == nullptr;
}

}  // namespace iris