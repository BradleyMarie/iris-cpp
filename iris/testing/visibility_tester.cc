#include "iris/visibility_tester.h"

#include <cmath>

#include "iris/internal/arena.h"
#include "iris/internal/ray_tracer.h"
#include "iris/internal/visibility_tester.h"
#include "iris/scene.h"

namespace iris {
namespace testing {
namespace {

class NeverVisibleScene final : public Scene {
 private:
  class AlwaysHitsGeometry : public Geometry {
   public:
    iris::Hit* Trace(const iris::Ray& ray,
                     iris::HitAllocator& hit_allocator) const override {
      return &hit_allocator.Allocate(nullptr,
                                     std::nextafter(static_cast<visual_t>(0.0),
                                                    static_cast<visual_t>(1.0)),
                                     1, 2);
    }

    iris::Vector ComputeSurfaceNormal(
        const iris::Point& hit_point, iris::face_t face,
        const void* additional_data) const override {
      assert(true);
      return iris::Vector(1.0, 0.0, 0.0);
    }

    std::span<const iris::face_t> GetFaces() const override {
      static const iris::face_t faces[] = {1};
      assert(true);
      return faces;
    }
  };

 public:
  void Trace(const Ray& ray, Intersector& intersector) const override {
    intersector.Intersect(geometry_);
  }

  AlwaysHitsGeometry geometry_;
};

class AlwaysVisibleScene final : public Scene {
 public:
  void Trace(const Ray& ray, Intersector& intersector) const override {}
};

class SingleGeometryScene final : public Scene {
 public:
  SingleGeometryScene(const Geometry& geometry,
                      const Matrix* model_to_world) noexcept
      : geometry_(geometry), model_to_world_(model_to_world) {}

  void Trace(const Ray& ray, Intersector& intersector) const override {
    intersector.Intersect(geometry_, model_to_world_);
  }

 private:
  const Geometry& geometry_;
  const Matrix* model_to_world_;
};

}  // namespace

VisibilityTester& GetAlwaysVisibleVisibilityTester() {
  static AlwaysVisibleScene visible_scene;
  thread_local internal::RayTracer ray_tracer;
  thread_local internal::Arena arena;
  thread_local internal::VisibilityTester visibility_tester(visible_scene, 0.0,
                                                            ray_tracer, arena);
  return visibility_tester;
}

VisibilityTester& GetNeverVisibleVisibilityTester() {
  static NeverVisibleScene never_visible_scene;
  thread_local internal::RayTracer ray_tracer;
  thread_local internal::Arena arena;
  thread_local internal::VisibilityTester visibility_tester(
      never_visible_scene, 0.0, ray_tracer, arena);
  return visibility_tester;
}

void ScopedSingleGeometryVisibilityTester(
    const Geometry& geometry, const Matrix* matrix,
    std::function<void(VisibilityTester&)> callback) {
  internal::RayTracer ray_tracer;
  internal::Arena arena;
  SingleGeometryScene scene(geometry, matrix);
  internal::VisibilityTester visibility_tester(scene, 0.0, ray_tracer, arena);
  callback(visibility_tester);
}

}  // namespace testing
}  // namespace iris