#ifndef _FRONTENDS_PBRT_INTEGRATORS_LIGHT_SCENES_
#define _FRONTENDS_PBRT_INTEGRATORS_LIGHT_SCENES_

#include <memory>
#include <string_view>

#include "iris/light_scene.h"

namespace iris::pbrt_frontend::integrators {

std::unique_ptr<LightScene::Builder> ParseLightScene(std::string_view type);

}  // namespace iris::pbrt_frontend::integrators

#endif  // _FRONTENDS_PBRT_INTEGRATORS_LIGHT_SCENES_