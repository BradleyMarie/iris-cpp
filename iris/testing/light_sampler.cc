#include "iris/testing/light_sampler.h"

#include "iris/internal/arena.h"
#include "iris/light_sample_allocator.h"
#include "iris/light_scene.h"
#include "iris/random/mock_random.h"

namespace iris {
namespace testing {
namespace {

class EmptyLightScene : public LightScene {
 public:
  LightSample* Sample(const Point& hit_point, Random& rng,
                      LightSampleAllocator& allocator) const override {
    return nullptr;
  }
};

}  // namespace

LightSampler& GetEmptyLightSampler() {
  static EmptyLightScene empty_scene;
  static iris::random::MockRandom rng;
  thread_local internal::Arena arena;
  thread_local LightSampleAllocator light_sample_allocator(arena);
  thread_local LightSampler light_sampler(empty_scene, rng,
                                          light_sample_allocator);
  return light_sampler;
}

}  // namespace testing
}  // namespace iris