#include "iris/light_scenes/one_light_scene.h"

namespace iris {
namespace light_scenes {

std::unique_ptr<LightScene::Builder> OneLightScene::Builder::Create() {
  return std::make_unique<OneLightScene::Builder>();
}

std::unique_ptr<LightScene> OneLightScene::Builder::Build(
    const SceneObjects& scene_objects) const {
  return std::make_unique<OneLightScene>(scene_objects);
}

LightSample* OneLightScene::Sample(const Point& hit_point, Random& rng,
                                   LightSampleAllocator& allocator) const {
  if (scene_objects_.NumLights() == 0) {
    return nullptr;
  }

  if (scene_objects_.NumLights() == 1) {
    return &allocator.Allocate(scene_objects_.GetLight(0), std::nullopt);
  }

  auto index = rng.NextIndex(scene_objects_.NumLights());
  return &allocator.Allocate(scene_objects_.GetLight(index), pdf_);
}

}  // namespace light_scenes
}  // namespace iris