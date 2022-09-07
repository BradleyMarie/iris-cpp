#include "iris/testing/mock_visibility_tester.h"

namespace iris {
namespace testing {

void MockVisibilityTester::Trace(const Ray& ray,
                                 Intersector& intersector) const {
  bool visible = Visible(ray, intersector.MaximumDistance());
  if (!visible) {
    geometry_.distance_ =
        (intersector.MinimumDistance() + intersector.MaximumDistance()) / 2.0;
    intersector.Intersect(geometry_);
  }
}

}  // namespace testing
}  // namespace iris