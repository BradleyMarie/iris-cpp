#ifndef _IRIS_INTERNAL_VISIBILITY_TESTER_
#define _IRIS_INTERNAL_VISIBILITY_TESTER_

#include <optional>

#include "iris/float.h"
#include "iris/geometry.h"
#include "iris/integer.h"
#include "iris/matrix.h"
#include "iris/spectrum.h"
#include "iris/visibility_tester.h"

namespace iris {
namespace internal {

class VisibilityTester : public iris::VisibilityTester {
 public:
  VisibilityTester(const Scene& scene, geometric_t minimum_distance,
                   internal::RayTracer& ray_tracer)
      : iris::VisibilityTester(scene, minimum_distance, ray_tracer) {}

  struct VisibleResult {
    const Spectrum& emission;
    Point hit_point;
  };

  std::optional<VisibleResult> Visible(const Ray& ray, const Geometry& geometry,
                                       const Matrix* model_to_world,
                                       face_t face, visual_t geometry_area,
                                       visual_t* pdf);

 private:
  VisibilityTester(const VisibilityTester&) = delete;
  VisibilityTester& operator=(const VisibilityTester&) = delete;

  bool Intersects(const Ray& ray, const Geometry& geometry,
                  const Matrix* model_to_world, face_t face);
};

}  // namespace internal
}  // namespace iris

#endif  // _IRIS_INTERNAL_VISIBILITY_TESTER_