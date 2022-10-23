#include "iris/light_scenes/all_light_scene.h"

namespace iris {
namespace light_scenes {

std::unique_ptr<LightScene::Builder> AllLightScene::Builder::Create() {
  return std::make_unique<AllLightScene::Builder>();
}

std::unique_ptr<LightScene> AllLightScene::Builder::Build(
    const SceneObjects& scene_objects) const {
  return std::make_unique<AllLightScene>(scene_objects);
}

LightSample* AllLightScene::Sample(const Point& hit_point, Random& rng,
                                   LightSampleAllocator& allocator) const {
  LightSample* next = nullptr;
  for (size_t i = 0; i < scene_objects_.NumLights(); i++) {
    next = &allocator.Allocate(scene_objects_.GetLight(i), std::nullopt, next);
  }
  return next;
}

}  // namespace light_scenes
}  // namespace iris