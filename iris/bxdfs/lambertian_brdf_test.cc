#include "iris/bxdfs/lambertian_brdf.h"

#define _USE_MATH_CONSTANTS
#include <cmath>

#include "googletest/include/gtest/gtest.h"
#include "iris/random/mock_random.h"
#include "iris/reflectors/mock_reflector.h"
#include "iris/testing/bxdf_tester.h"

TEST(LambertianBrdfTest, Sample) {
  iris::reflectors::MockReflector reflector;
  iris::random::MockRandom rng;
  EXPECT_CALL(rng, NextGeometric())
      .Times(2)
      .WillRepeatedly(testing::Return(0.0));

  iris::bxdfs::LambertianBrdf bxdf(reflector);
  auto result = bxdf.Sample(iris::Vector(0.0, 0.0, -1.0), rng);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result);
}

TEST(LambertianBrdfTest, PdfTransmitted) {
  iris::reflectors::MockReflector reflector;
  iris::bxdfs::LambertianBrdf bxdf(reflector);
  EXPECT_EQ(0.0, bxdf.Pdf(iris::Vector(0.0, 0.0, -1.0),
                          iris::Vector(0.0, 0.0, -1.0)));
}

TEST(LambertianBrdfTest, PdfReflected) {
  iris::reflectors::MockReflector reflector;
  iris::bxdfs::LambertianBrdf bxdf(reflector);
  EXPECT_EQ(
      1.0, bxdf.Pdf(iris::Vector(0.0, 0.0, -1.0), iris::Vector(0.0, 0.0, 1.0)));
}

TEST(LambertianBrdfTest, ReflectanceBtdf) {
  iris::testing::BxdfTester tester;

  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::bxdfs::LambertianBrdf bxdf(reflector);
  auto* result =
      tester.Reflectance(bxdf, iris::Vector(0.0, 0.0, 1.0),
                         iris::Vector(0.0, 0.0, -1.0), iris::Bxdf::BTDF);
  ASSERT_FALSE(result);
}

TEST(LambertianBrdfTest, ReflectanceTransmitted) {
  iris::testing::BxdfTester tester;

  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::bxdfs::LambertianBrdf bxdf(reflector);
  auto* result =
      tester.Reflectance(bxdf, iris::Vector(0.0, 0.0, 1.0),
                         iris::Vector(0.0, 0.0, 1.0), iris::Bxdf::BRDF);
  ASSERT_FALSE(result);
}

TEST(LambertianBrdfTest, Reflectance) {
  iris::testing::BxdfTester tester;

  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::bxdfs::LambertianBrdf bxdf(reflector);
  auto* result =
      tester.Reflectance(bxdf, iris::Vector(0.0, 0.0, 1.0),
                         iris::Vector(0.0, 0.0, -1.0), iris::Bxdf::BRDF);
  ASSERT_TRUE(result);
  EXPECT_NEAR(M_1_PI, result->Reflectance(1.0), 0.0001);
}