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

OneLightScene::OneLightScene(const SceneObjects& scene_objects) noexcept
    : scene_objects_(scene_objects),
      pdf_((scene_objects.NumLights() > 1)
               ? std::optional<visual_t>(
                     static_cast<intermediate_t>(1.0) /
                     static_cast<intermediate_t>(scene_objects.NumLights()))
               : std::nullopt) {}

LightSample* OneLightScene::Sample(const Point& hit_point, Random& rng,
                                   LightSampleAllocator& allocator) const {
  if (scene_objects_.NumLights() == 0) {
    return nullptr;
  }

  size_t index = 0;
  if (scene_objects_.NumLights() != 1) {
    index = rng.NextIndex(scene_objects_.NumLights());
  }

  return &allocator.Allocate(scene_objects_.GetLight(index), pdf_);
}

}  // namespace light_scenes
}  // namespace iris