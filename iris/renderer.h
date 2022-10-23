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
#include "iris/scene_objects.h"

namespace iris {

class Renderer {
 public:
  Renderer(const Scene::Builder& scene_builder,
           const LightScene::Builder& light_scene_builder,
           SceneObjects scene_objects)
      : scene_(scene_builder.Build(scene_objects)),
        light_scene_(light_scene_builder.Build(scene_objects)),
        scene_objects_(std::move(scene_objects)) {}

  Framebuffer Render(const Camera& camera, const ImageSampler& image_sampler,
                     const Integrator& integrator,
                     const ColorMatcher& color_matcher, Random& rng,
                     std::pair<size_t, size_t> image_dimensions,
                     geometric_t minimum_distance,
                     unsigned num_threads = 0) const;

 private:
  const std::unique_ptr<const Scene> scene_;
  const std::unique_ptr<const LightScene> light_scene_;
  const SceneObjects scene_objects_;
};

}  // namespace iris

#endif  // _IRIS_RENDERER_