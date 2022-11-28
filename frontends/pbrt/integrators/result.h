#ifndef _FRONTENDS_PBRT_INTEGRATORS_RESULT_
#define _FRONTENDS_PBRT_INTEGRATORS_RESULT_

#include <array>
#include <memory>
#include <optional>

#include "iris/integrator.h"
#include "iris/light_scene.h"

namespace iris::pbrt_frontend::integrators {

struct Result {
  std::unique_ptr<iris::Integrator> integrator;
  std::unique_ptr<iris::LightScene::Builder> light_scene_builder;
  std::optional<std::array<size_t, 4>> pixel_bounds;
};

}  // namespace iris::pbrt_frontend::integrators

#endif  // _FRONTENDS_PBRT_INTEGRATORS_RESULT_