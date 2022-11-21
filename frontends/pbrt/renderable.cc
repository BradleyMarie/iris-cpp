#include "frontends/pbrt/renderable.h"

#include <cassert>

namespace iris::pbrt_frontend {

Renderable::Renderable(Renderer renderer, std::unique_ptr<Camera> camera,
                       std::unique_ptr<ImageSampler> image_sampler,
                       std::unique_ptr<Integrator> integrator,
                       std::unique_ptr<Random> rng,
                       std::pair<size_t, size_t> image_dimensions) noexcept
    : renderer_(std::move(renderer)),
      camera_(std::move(camera)),
      image_sampler_(std::move(image_sampler)),
      integrator_(std::move(integrator)),
      rng_(std::move(rng)),
      image_dimensions_(image_dimensions) {
  assert(camera_);
  assert(image_sampler_);
  assert(integrator_);
  assert(rng_);
  assert(image_dimensions.first != 0 && image_dimensions.second != 0);
}

Framebuffer Renderable::Render(const ColorMatcher& color_matcher,
                               geometric_t minimum_distance,
                               unsigned num_threads) const {
  return renderer_.Render(*camera_, *image_sampler_, *integrator_,
                          color_matcher, *rng_, image_dimensions_,
                          minimum_distance, num_threads);
}

}  // namespace iris::pbrt_frontend