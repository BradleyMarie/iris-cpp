#include "iris/image_samplers/random_image_sampler.h"

#include <limits>

#include "googletest/include/gtest/gtest.h"
#include "iris/random/mock_random.h"

TEST(RandomImageSamplerTest, SamplesPerPixel) {
  iris::image_samplers::RandomImageSampler sampler(5);
  EXPECT_EQ(5u, sampler.SamplesPerPixel());
}

TEST(RandomImageSamplerTest, Duplicate) {
  iris::image_samplers::RandomImageSampler sampler(5);
  EXPECT_EQ(5u, sampler.SamplesPerPixel());
  auto duplicate = sampler.Duplicate();
  EXPECT_EQ(5u, duplicate->SamplesPerPixel());
}

TEST(RandomImageSamplerTest, SampleNoLens) {
  iris::image_samplers::RandomImageSampler sampler(5);
  EXPECT_EQ(5u, sampler.SamplesPerPixel());

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, min())
      .WillRepeatedly(testing::Return(
          std::numeric_limits<iris::Random::result_type>::min()));
  EXPECT_CALL(rng, max())
      .WillRepeatedly(testing::Return(
          std::numeric_limits<iris::Random::result_type>::max()));
  iris::Random::result_type result = 0;
  EXPECT_CALL(rng, Invoke()).WillRepeatedly(testing::Invoke([&]() {
    return result++;
  }));

  auto sample = sampler.SamplePixel(std::make_pair(2, 2), std::make_pair(0, 1),
                                    0, false, rng);
  EXPECT_TRUE(sample.image_uv[0] >= 0.5);
  EXPECT_TRUE(sample.image_uv[0] < 1.0);
  EXPECT_TRUE(sample.image_uv[1] >= 0.0);
  EXPECT_TRUE(sample.image_uv[1] < 0.5);
  EXPECT_FALSE(sample.lens_uv);
  EXPECT_EQ(&rng, &sample.rng);
}

TEST(RandomImageSamplerTest, SampleWithLens) {
  iris::image_samplers::RandomImageSampler sampler(5);
  EXPECT_EQ(5u, sampler.SamplesPerPixel());

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, min())
      .WillRepeatedly(testing::Return(
          std::numeric_limits<iris::Random::result_type>::min()));
  EXPECT_CALL(rng, max())
      .WillRepeatedly(testing::Return(
          std::numeric_limits<iris::Random::result_type>::max()));
  iris::Random::result_type result = 0;
  EXPECT_CALL(rng, Invoke()).WillRepeatedly(testing::Invoke([&]() {
    return result++;
  }));

  auto sample = sampler.SamplePixel(std::make_pair(2, 2), std::make_pair(0, 1),
                                    0, true, rng);
  EXPECT_TRUE(sample.image_uv[0] >= 0.5);
  EXPECT_TRUE(sample.image_uv[0] < 1.0);
  EXPECT_TRUE(sample.image_uv[1] >= 0.0);
  EXPECT_TRUE(sample.image_uv[1] < 0.5);
  ASSERT_TRUE(sample.lens_uv);
  EXPECT_TRUE((*sample.lens_uv)[0] >= 0.0);
  EXPECT_TRUE((*sample.lens_uv)[0] < 1.0);
  EXPECT_TRUE((*sample.lens_uv)[1] >= 0.0);
  EXPECT_TRUE((*sample.lens_uv)[1] < 1.0);
  EXPECT_EQ(&rng, &sample.rng);
}