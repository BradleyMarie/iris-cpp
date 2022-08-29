#include "iris/light_sampler.h"

namespace iris {

LightSample* LightSampler::Sample(const Point& hit_point) {
  return scene_.Sample(hit_point, rng_, allocator_);
}

}  // namespace