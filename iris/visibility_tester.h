#ifndef _IRIS_VISIBILITY_TESTER_
#define _IRIS_VISIBILITY_TESTER_

#include <limits>

#include "iris/float.h"
#include "iris/ray.h"
#include "iris/scene.h"

namespace iris {
namespace internal {
class RayTracer;
}  // namespace internal

class VisibilityTester final {
 public:
  VisibilityTester(const Scene& scene, geometric_t minimum_distance,
                   internal::RayTracer& ray_tracer)
      : scene_(scene),
        minimum_distance_(minimum_distance),
        ray_tracer_(ray_tracer) {}

  bool Visible(const Ray& ray,
               geometric_t maximum_distance =
                   std::numeric_limits<geometric_t>::infinity());

 private:
  const Scene& scene_;
  geometric_t minimum_distance_;
  internal::RayTracer& ray_tracer_;
};

}  // namespace iris

#endif  // _IRIS_VISIBILITY_TESTER_