#include "iris/bxdfs/lambertian_brdf.h"

#define _USE_MATH_CONSTANTS
#include <cmath>

#include "googletest/include/gtest/gtest.h"
#include "iris/random/mock_random.h"
#include "iris/reflectors/mock_reflector.h"
#include "iris/testing/spectral_allocator.h"

TEST(LambertianBrdfTest, Sample) {
  iris::reflectors::MockReflector reflector;
  iris::random::MockRandom rng;
  EXPECT_CALL(rng, NextGeometric())
      .Times(2)
      .WillRepeatedly(testing::Return(0.0));
  iris::Sampler sampler(rng);

  iris::bxdfs::LambertianBrdf bxdf(reflector);
  auto result = bxdf.Sample(iris::Vector(0.0, 0.0, -1.0), sampler);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result);
}

TEST(LambertianBrdfTest, DiffusePdfTransmitted) {
  iris::reflectors::MockReflector reflector;
  iris::bxdfs::LambertianBrdf bxdf(reflector);
  EXPECT_EQ(0.0,
            bxdf.Pdf(iris::Vector(0.0, 0.0, -1.0), iris::Vector(0.0, 0.0, -1.0),
                     iris::Bxdf::SampleSource::BXDF));
}

TEST(LambertianBrdfTest, DiffusePdfReflected) {
  iris::reflectors::MockReflector reflector;
  iris::bxdfs::LambertianBrdf bxdf(reflector);
  EXPECT_EQ(1.0,
            bxdf.Pdf(iris::Vector(0.0, 0.0, -1.0), iris::Vector(0.0, 0.0, 1.0),
                     iris::Bxdf::SampleSource::BXDF));
}

TEST(LambertianBrdfTest, ReflectanceBtdf) {
  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::bxdfs::LambertianBrdf bxdf(reflector);
  auto* result = bxdf.Reflectance(
      iris::Vector(0.0, 0.0, 1.0), iris::Vector(0.0, 0.0, -1.0),
      iris::Bxdf::SampleSource::BXDF, iris::Bxdf::Hemisphere::BTDF,
      iris::testing::GetSpectralAllocator());
  ASSERT_FALSE(result);
}

TEST(LambertianBrdfTest, ReflectanceTransmitted) {
  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::bxdfs::LambertianBrdf bxdf(reflector);
  auto* result = bxdf.Reflectance(
      iris::Vector(0.0, 0.0, 1.0), iris::Vector(0.0, 0.0, 1.0),
      iris::Bxdf::SampleSource::BXDF, iris::Bxdf::Hemisphere::BRDF,
      iris::testing::GetSpectralAllocator());
  ASSERT_FALSE(result);
}

TEST(LambertianBrdfTest, Reflectance) {
  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::bxdfs::LambertianBrdf bxdf(reflector);
  auto* result = bxdf.Reflectance(
      iris::Vector(0.0, 0.0, 1.0), iris::Vector(0.0, 0.0, -1.0),
      iris::Bxdf::SampleSource::BXDF, iris::Bxdf::Hemisphere::BRDF,
      iris::testing::GetSpectralAllocator());
  ASSERT_TRUE(result);
  EXPECT_NEAR(M_1_PI, result->Reflectance(1.0), 0.0001);
}