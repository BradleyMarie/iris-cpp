#include "iris/testing/light_scene_tester.h"

namespace iris {
namespace testing {

LightSample* LightSceneTester::Sample(const LightScene& scene,
                                      const Point& hit_point, Random& rng) {
  LightSampleAllocator allocator(arena_);
  return scene.Sample(hit_point, rng, allocator);
}

}  // namespace testing
}  // namespace iris