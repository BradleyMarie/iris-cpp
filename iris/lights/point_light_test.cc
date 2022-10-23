#include "iris/lights/point_light.h"

#include <limits>
#include <memory>

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "iris/float.h"
#include "iris/random/mock_random.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/light_tester.h"
#include "iris/testing/mock_visibility_tester.h"

TEST(PointLightTest, SampleHits) {
  iris::testing::LightTester light_tester;
  iris::testing::MockVisibilityTester visibility_tester;
  iris::random::MockRandom random;

  auto spectrum = iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();

  EXPECT_CALL(visibility_tester,
              Visible(testing::Eq(iris::Ray(iris::Point(0.0, 0.0, -1.0),
                                            iris::Vector(0.0, 0.0, 1.0))),
                      testing::_))
      .WillOnce(testing::Return(true));

  iris::lights::PointLight light(iris::Point(0.0, 0.0, 0.0), spectrum);
  auto result = light_tester.Sample(light, iris::Point(0.0, 0.0, -1.0), random,
                                    visibility_tester);
  EXPECT_TRUE(result);
  EXPECT_EQ(spectrum.Get(), &result->emission);
  EXPECT_FALSE(result->pdf);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result->to_light);
}

TEST(PointLightTest, SampleMisses) {
  iris::testing::LightTester light_tester;
  iris::testing::MockVisibilityTester visibility_tester;
  iris::random::MockRandom random;

  auto spectrum = iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();

  EXPECT_CALL(visibility_tester,
              Visible(testing::Eq(iris::Ray(iris::Point(0.0, 0.0, -1.0),
                                            iris::Vector(0.0, 0.0, 1.0))),
                      testing::_))
      .WillOnce(testing::Return(false));

  iris::lights::PointLight light(iris::Point(0.0, 0.0, 0.0), spectrum);
  EXPECT_FALSE(light_tester.Sample(light, iris::Point(0.0, 0.0, -1.0), random,
                                   visibility_tester));
}

TEST(PointLightTest, Emission) {
  iris::testing::LightTester light_tester;
  iris::testing::MockVisibilityTester visibility_tester;

  auto spectrum = iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();

  iris::lights::PointLight light(iris::Point(0.0, 0.0, 0.0), spectrum);
  EXPECT_EQ(nullptr,
            light_tester.Emission(light,
                                  iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                            iris::Vector(0.0, 0.0, 1.0)),
                                  visibility_tester));
}