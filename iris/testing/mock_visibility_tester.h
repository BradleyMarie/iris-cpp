#ifndef _IRIS_TESTING_VISIBILITY_TESTER_
#define _IRIS_TESTING_VISIBILITY_TESTER_

#include "googlemock/include/gmock/gmock.h"
#include "iris/float.h"
#include "iris/intersector.h"
#include "iris/ray.h"
#include "iris/scene.h"

namespace iris {
namespace testing {

class MockVisibilityTester : public Scene {
 public:
  MockVisibilityTester() : geometry_(0.0) {}

  const_iterator begin() const override { return end(); }
  void Trace(const Ray& ray, Intersector& intersector) const final override;

  MOCK_METHOD(bool, Visible, (const Ray&, geometric_t), (const));

 private:
  class MockGeometry : public Geometry {
   public:
    MockGeometry(geometric_t distance) : distance_(distance) {}

    Hit* Trace(const Ray& ray, HitAllocator& hit_allocator) const override {
      return &hit_allocator.Allocate(nullptr, distance_, 1u, 2u);
    }

    Vector ComputeSurfaceNormal(const Point& hit_point, face_t face,
                                const void* additional_data) const override {
      return Vector(1.0, 0.0, 0.0);
    }

    std::span<const iris::face_t> GetFaces() const override {
      static const iris::face_t faces[] = {1u, 2u};
      return faces;
    }

    geometric_t distance_;
  };

  mutable MockGeometry geometry_;
};

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_VISIBILITY_TESTER_