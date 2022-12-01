#ifndef _IRIS_LIGHT_SCENES_ONE_LIGHT_SCENE_
#define _IRIS_LIGHT_SCENES_ONE_LIGHT_SCENE_

#include <memory>
#include <optional>

#include "iris/light_sample.h"
#include "iris/light_sample_allocator.h"
#include "iris/light_scene.h"
#include "iris/point.h"
#include "iris/random.h"
#include "iris/scene_objects.h"

namespace iris {
namespace light_scenes {

class OneLightScene final : public LightScene {
 public:
  class Builder final : public LightScene::Builder {
   public:
    static std::unique_ptr<LightScene::Builder> Create();
    std::unique_ptr<LightScene> Build(
        const SceneObjects& scene_objects) const override;
  };

  OneLightScene(const SceneObjects& scene_objects) noexcept;

  LightSample* Sample(const Point& hit_point, Random& rng,
                      LightSampleAllocator& allocator) const override;

 private:
  const SceneObjects& scene_objects_;
  const std::optional<visual_t> pdf_;
};

}  // namespace light_scenes
}  // namespace iris

#endif  // _IRIS_LIGHT_SCENES_ONE_LIGHT_SCENE_