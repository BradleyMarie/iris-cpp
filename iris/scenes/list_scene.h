#ifndef _IRIS_SCENES_LIST_SCENE_
#define _IRIS_SCENES_LIST_SCENE_

#include <memory>

#include "iris/intersector.h"
#include "iris/ray.h"
#include "iris/scene.h"
#include "iris/scene_objects.h"

namespace iris {
namespace scenes {

class ListScene final : public Scene {
 public:
  class Builder final : public Scene::Builder {
   public:
    static std::unique_ptr<Scene::Builder> Create();
    std::unique_ptr<Scene> Build(
        const SceneObjects& scene_objects) const override;
  };

  ListScene(const SceneObjects& scene_objects) noexcept
      : scene_objects_(scene_objects) {}

  void Trace(const Ray& ray, Intersector& intersector) const override;

 private:
  const SceneObjects& scene_objects_;
};

}  // namespace scenes
}  // namespace iris

#endif  // _IRIS_SCENES_LIST_SCENE_