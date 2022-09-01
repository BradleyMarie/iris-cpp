#ifndef _IRIS_TESTING_SCENE_
#define _IRIS_TESTING_SCENE_

#include <functional>
#include <memory>
#include <vector>

#include "iris/float.h"
#include "iris/geometry.h"
#include "iris/hit.h"
#include "iris/internal/hit_arena.h"
#include "iris/matrix.h"
#include "iris/ray.h"
#include "iris/scene.h"

namespace iris {
namespace testing {

class SceneTester {
 public:
  struct Hit {
    const Geometry& geometry;
    const Matrix* model_to_world;
    geometric_t distance;
  };

  void Trace(const Scene& scene, const Ray& ray, geometric_t min_distance,
             geometric_t max_distance,
             std::function<void(const std::vector<Hit>& hits)> validator);

 private:
  internal::HitArena hit_arena_;
};

void SceneTestSuite(Scene::Builder& scene_builder);

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_SCENE_