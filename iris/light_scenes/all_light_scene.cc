#include "iris/light_scenes/all_light_scene.h"

#include <limits>

namespace iris {
namespace light_scenes {

std::unique_ptr<LightScene> AllLightScene::Builder::Build(
    std::vector<std::unique_ptr<Light>> lights) {
  return std::make_unique<AllLightScene>(std::move(lights));
}

LightSample* AllLightScene::Sample(const Point& hit_point, Random& rng,
                                   LightSampleAllocator& allocator) const {
  LightSample* next = nullptr;
  for (const auto& light : lights_) {
    next = &allocator.Allocate(*light,
                               std::numeric_limits<visual_t>::infinity(), next);
  }
  return next;
}

}  // namespace light_scenes
}  // namespace iris