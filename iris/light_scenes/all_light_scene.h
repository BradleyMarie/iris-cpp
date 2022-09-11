#ifndef _IRIS_LIGHT_SCENES_MOCK_LIGHT_SCENE_
#define _IRIS_LIGHT_SCENES_MOCK_LIGHT_SCENE_

#include <memory>

#include "iris/light.h"
#include "iris/light_sample.h"
#include "iris/light_sample_allocator.h"
#include "iris/light_scene.h"
#include "iris/point.h"
#include "iris/random.h"

namespace iris {
namespace light_scenes {

class AllLightScene final : public LightScene {
 public:
  class Builder final : public LightScene::Builder {
   public:
    static std::unique_ptr<LightScene::Builder> Create() {
      return std::make_unique<Builder>();
    }

   private:
    std::unique_ptr<LightScene> Build(
        std::vector<std::unique_ptr<Light>> lights) override;
  };

  AllLightScene(std::vector<std::unique_ptr<Light>> lights)
      : lights_(std::move(lights)) {}

  LightSample* Sample(const Point& hit_point, Random& rng,
                      LightSampleAllocator& allocator) const override;

 private:
  std::vector<std::unique_ptr<Light>> lights_;
};

}  // namespace light_scenes
}  // namespace iris

#endif  // _IRIS_LIGHT_SCENES_MOCK_LIGHT_SCENE_