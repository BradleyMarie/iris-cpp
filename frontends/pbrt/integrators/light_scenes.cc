#include "frontends/pbrt/integrators/light_scenes.h"

#include <unordered_map>

#include "iris/light_scenes/all_light_scene.h"
#include "iris/light_scenes/one_light_scene.h"

namespace iris::pbrt_frontend::integrators {
namespace {

std::unique_ptr<LightScene::Builder> MakeAllLightSampler() {
  return std::make_unique<iris::light_scenes::AllLightScene::Builder>();
}

std::unique_ptr<LightScene::Builder> MakeUniformLightSampler() {
  return std::make_unique<iris::light_scenes::OneLightScene::Builder>();
}

}  // namespace

std::unique_ptr<LightScene::Builder> ParseLightScene(std::string_view type) {
  static const std::unordered_map<std::string_view,
                                  std::unique_ptr<LightScene::Builder> (*)()>
      g_light_scenes = {
          {"all", MakeAllLightSampler},
          {"uniform", MakeUniformLightSampler},
      };

  auto iter = g_light_scenes.find(type);
  if (iter == g_light_scenes.end()) {
    std::cerr << "ERROR: Unsupported value for parameter lightsamplestrategy: "
              << type << std::endl;
    exit(EXIT_FAILURE);
  }

  return iter->second();
}

}  // namespace iris::pbrt_frontend::integrators