#ifndef _IRIS_RENDERER_
#define _IRIS_RENDERER_

#include <memory>
#include <utility>

#include "iris/camera.h"
#include "iris/color_matcher.h"
#include "iris/framebuffer.h"
#include "iris/image_sampler.h"
#include "iris/integrator.h"
#include "iris/light_scene.h"
#include "iris/scene.h"

namespace iris {

class Renderer {
 public:
  Renderer(Scene::Builder& scene_builder,
           LightScene::Builder& light_scene_builder)
      : scene_(scene_builder.Build()),
        light_scene_(light_scene_builder.Build(*scene_)) {}

  Framebuffer Render(const Camera& camera, const ImageSampler& image_sampler,
                     const Integrator& integrator,
                     const ColorMatcher& color_matcher, Random& rng,
                     std::pair<size_t, size_t> image_dimensions,
                     geometric_t minimum_distance,
                     unsigned num_threads = 0) const;

 private:
  std::unique_ptr<const Scene> scene_;
  std::unique_ptr<const LightScene> light_scene_;
};

}  // namespace iris

#endif  // _IRIS_RENDERER_