#include "iris/ray_tracer.h"

#include "iris/internal/arena.h"
#include "iris/internal/ray_tracer.h"
#include "iris/scene.h"

namespace iris {
namespace testing {
namespace {

class NoHitsScene final : public Scene {
 public:
  void Trace(const Ray& ray, Intersector& intersector) const override {}
};

}  // namespace

RayTracer& GetNoHitsRayTracer() {
  static NoHitsScene no_hits_scene;
  thread_local internal::RayTracer internal_ray_tracer;
  thread_local internal::Arena arena;
  thread_local RayTracer ray_tracer(no_hits_scene, nullptr, 0.0,
                                    internal_ray_tracer, arena);
  return ray_tracer;
}

}  // namespace testing
}  // namespace iris