#include "iris/light_sampler.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/arena.h"

const iris::Light* g_light = reinterpret_cast<const iris::Light*>(1);
iris::Random* g_random = reinterpret_cast<iris::Random*>(2);

class TestLightScene final : public iris::LightScene {
 public:
  TestLightScene(const iris::Point& hit_point, iris::Random& rng,
                 iris::LightSampleAllocator& allocator)
      : expected_hit_point_(hit_point),
        expected_rng_(rng),
        allocator_(allocator) {}

  iris::LightSample* Sample(
      const iris ::Point& hit_point, iris::Random& rng,
      iris ::LightSampleAllocator& allocator) const override {
    EXPECT_EQ(expected_hit_point_, hit_point);
    EXPECT_EQ(&expected_rng_, &rng);
    EXPECT_EQ(&allocator_, &allocator);
    return &allocator.Allocate(*g_light, 2.0);
  }

  iris::Point expected_hit_point_;
  iris::Random& expected_rng_;
  iris::LightSampleAllocator& allocator_;
};

TEST(LightSamplerTest, Sample) {
  iris::internal::Arena arena;
  iris::LightSampleAllocator allocator(arena);

  TestLightScene scene(iris::Point(1.0, 1.0, 1.0), *g_random, allocator);
  iris::LightSampler sampler(scene, *g_random, allocator);

  auto sample = sampler.Sample(iris::Point(1.0, 1.0, 1.0));
  ASSERT_NE(nullptr, sample);
  EXPECT_EQ(g_light, &sample->light);
  EXPECT_EQ(2.0, sample->pdf);
  EXPECT_EQ(nullptr, sample->next);
}