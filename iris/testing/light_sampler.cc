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

class ListLightScene : public LightScene {
 public:
  ListLightScene(std::span<const LightSampleListEntry> sample_list)
      : sample_list_(sample_list) {}

  LightSample* Sample(const Point& hit_point, Random& rng,
                      LightSampleAllocator& allocator) const override {
    LightSample* result = nullptr;
    for (auto iter = sample_list_.rbegin(); iter != sample_list_.rend();
         iter++) {
      if (!iter->light) {
        continue;
      }

      result = &allocator.Allocate(*iter->light, iter->pdf, result);
    }
    return result;
  }

 private:
  std::span<const LightSampleListEntry> sample_list_;
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

void ScopedListLightSampler(std::span<const LightSampleListEntry> sample_list,
                            std::function<void(LightSampler&)> callback) {
  static iris::random::MockRandom rng;
  internal::Arena arena;
  LightSampleAllocator light_sample_allocator(arena);
  ListLightScene scene(sample_list);
  LightSampler light_sampler(scene, rng, light_sample_allocator);
  callback(light_sampler);
}

}  // namespace testing
}  // namespace iris