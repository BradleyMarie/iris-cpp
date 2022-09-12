#ifndef _IRIS_TESTING_LIGHT_SCENE_TESTER_
#define _IRIS_TESTING_LIGHT_SCENE_TESTER_

#include "iris/internal/arena.h"
#include "iris/light_sample.h"
#include "iris/light_sample_allocator.h"
#include "iris/light_scene.h"
#include "iris/point.h"
#include "iris/random.h"

namespace iris {
namespace testing {

class LightSceneTester final {
 public:
  LightSample* Sample(const LightScene& scene, const Point& hit_point,
                      Random& rng);

 private:
  internal::Arena arena_;
};

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_LIGHT_SCENE_TESTER_