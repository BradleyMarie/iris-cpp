#include "iris/lights/point_light.h"

#include <limits>
#include <memory>

#include "googlemock/include/gmock/gmock.h"
#include "googletest/include/gtest/gtest.h"
#include "iris/float.h"
#include "iris/random/mock_random.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/spectral_allocator.h"
#include "iris/testing/visibility_tester.h"

TEST(PointLightTest, SampleHits) {
  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));

  auto spectrum = iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();

  iris::lights::PointLight light(iris::Point(0.0, 0.0, 0.0), spectrum);
  auto result = light.Sample(iris::Point(0.0, 0.0, -1.0), iris::Sampler(random),
                             iris::testing::GetAlwaysVisibleVisibilityTester(),
                             iris::testing::GetSpectralAllocator());
  EXPECT_TRUE(result);
  EXPECT_EQ(spectrum.Get(), &result->emission);
  EXPECT_FALSE(result->pdf);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result->to_light);
}

TEST(PointLightTest, SampleMisses) {
  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));

  auto spectrum = iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();

  iris::lights::PointLight light(iris::Point(0.0, 0.0, 0.0), spectrum);
  EXPECT_FALSE(light.Sample(iris::Point(0.0, 0.0, -1.0), iris::Sampler(random),
                            iris::testing::GetNeverVisibleVisibilityTester(),
                            iris::testing::GetSpectralAllocator()));
}

TEST(PointLightTest, Emission) {
  auto spectrum = iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();

  iris::lights::PointLight light(iris::Point(0.0, 0.0, 0.0), spectrum);

  iris::visual_t pdf;
  EXPECT_EQ(nullptr,
            light.Emission(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                     iris::Vector(0.0, 0.0, 1.0)),
                           iris::testing::GetAlwaysVisibleVisibilityTester(),
                           iris::testing::GetSpectralAllocator(), &pdf));
}