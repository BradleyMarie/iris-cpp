#include "iris/image_samplers/stratified_image_sampler.h"

#include <limits>

#include "googletest/include/gtest/gtest.h"
#include "iris/random/mock_random.h"

TEST(StraifiedImageSamplerTest, SamplesPerPixel) {
  iris::image_samplers::StratifiedImageSampler sampler(5, 3, false);
  EXPECT_EQ(15u, sampler.SamplesPerPixel());
}

TEST(RandomImageSamplerTest, Duplicate) {
  iris::image_samplers::StratifiedImageSampler sampler(5, 3, false);
  EXPECT_EQ(15u, sampler.SamplesPerPixel());
  auto duplicate = sampler.Duplicate();
  EXPECT_EQ(15u, duplicate->SamplesPerPixel());
}

TEST(RandomImageSamplerTest, SampleNoLensNoJitter) {
  iris::image_samplers::StratifiedImageSampler sampler(2, 2, false);
  EXPECT_EQ(4u, sampler.SamplesPerPixel());

  iris::random::MockRandom rng;
  auto sample = sampler.SamplePixel(std::make_pair(2, 2), std::make_pair(0, 1),
                                    0, false, rng);
  EXPECT_EQ(sample.image_uv[0], 0.625);
  EXPECT_EQ(sample.image_uv[1], 0.125);
  EXPECT_FALSE(sample.lens_uv);
  EXPECT_EQ(&rng, &sample.rng);
}

TEST(RandomImageSamplerTest, SampleWithLensNoJitter) {
  iris::image_samplers::StratifiedImageSampler sampler(2, 2, false);
  EXPECT_EQ(4u, sampler.SamplesPerPixel());

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, NextGeometric())
      .Times(2)
      .WillRepeatedly(testing::Return(0.1));

  auto sample = sampler.SamplePixel(std::make_pair(2, 2), std::make_pair(0, 1),
                                    0, true, rng);
  EXPECT_EQ(sample.image_uv[0], 0.625);
  EXPECT_EQ(sample.image_uv[1], 0.125);
  ASSERT_TRUE(sample.lens_uv);
  EXPECT_TRUE((*sample.lens_uv)[0] >= 0.0);
  EXPECT_TRUE((*sample.lens_uv)[0] < 1.0);
  EXPECT_TRUE((*sample.lens_uv)[1] >= 0.0);
  EXPECT_TRUE((*sample.lens_uv)[1] < 1.0);
  EXPECT_EQ(&rng, &sample.rng);
}

TEST(RandomImageSamplerTest, SampleNoLensWithJitter) {
  iris::image_samplers::StratifiedImageSampler sampler(2, 2, true);
  EXPECT_EQ(4u, sampler.SamplesPerPixel());

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, NextGeometric())
      .Times(2)
      .WillRepeatedly(testing::Return(0.1));

  auto sample = sampler.SamplePixel(std::make_pair(2, 2), std::make_pair(0, 1),
                                    0, false, rng);
  EXPECT_TRUE(sample.image_uv[0] >= 0.5);
  EXPECT_TRUE(sample.image_uv[0] < 0.75);
  EXPECT_TRUE(sample.image_uv[1] >= 0.0);
  EXPECT_TRUE(sample.image_uv[1] < 0.25);
  EXPECT_FALSE(sample.lens_uv);
  EXPECT_EQ(&rng, &sample.rng);
}

TEST(RandomImageSamplerTest, SampleWithLensWithJitter) {
  iris::image_samplers::StratifiedImageSampler sampler(2, 2, true);
  EXPECT_EQ(4u, sampler.SamplesPerPixel());

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, NextGeometric())
      .Times(4)
      .WillRepeatedly(testing::Return(0.1));

  auto sample = sampler.SamplePixel(std::make_pair(2, 2), std::make_pair(0, 1),
                                    0, true, rng);
  EXPECT_TRUE(sample.image_uv[0] >= 0.5);
  EXPECT_TRUE(sample.image_uv[0] < 0.75);
  EXPECT_TRUE(sample.image_uv[1] >= 0.0);
  EXPECT_TRUE(sample.image_uv[1] < 0.25);
  ASSERT_TRUE(sample.lens_uv);
  EXPECT_TRUE((*sample.lens_uv)[0] >= 0.0);
  EXPECT_TRUE((*sample.lens_uv)[0] < 1.0);
  EXPECT_TRUE((*sample.lens_uv)[1] >= 0.0);
  EXPECT_TRUE((*sample.lens_uv)[1] < 1.0);
  EXPECT_EQ(&rng, &sample.rng);
}