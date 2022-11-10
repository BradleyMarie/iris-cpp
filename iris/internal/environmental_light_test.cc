#include "iris/internal/environmental_light.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/environmental_lights/mock_environmental_light.h"
#include "iris/random/mock_random.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/spectral_allocator.h"
#include "iris/testing/visibility_tester.h"

TEST(EnvironmentalLightTest, EmissionFails) {
  auto environmental_light =
      std::make_unique<iris::environmental_lights::MockEnvironmentalLight>();
  EXPECT_CALL(*environmental_light,
              Emission(testing::_, testing::_, testing::_))
      .WillOnce(testing::Return(nullptr));

  iris::internal::EnvironmentalLight light(
      iris::ReferenceCounted(std::move(environmental_light)));

  EXPECT_EQ(nullptr,
            light.Emission(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                     iris::Vector(0.0, 0.0, 1.0)),
                           iris::testing::GetNeverVisibleVisibilityTester(),
                           iris::testing::GetSpectralAllocator()));
}

TEST(EnvironmentalLightTest, EmissionNotVisible) {
  iris::spectra::MockSpectrum spectrum;

  auto environmental_light =
      std::make_unique<iris::environmental_lights::MockEnvironmentalLight>();
  EXPECT_CALL(*environmental_light,
              Emission(testing::_, testing::_, testing::_))
      .WillOnce(testing::Return(&spectrum));

  iris::internal::EnvironmentalLight light(
      iris::ReferenceCounted(std::move(environmental_light)));

  EXPECT_EQ(nullptr,
            light.Emission(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                     iris::Vector(0.0, 0.0, 1.0)),
                           iris::testing::GetNeverVisibleVisibilityTester(),
                           iris::testing::GetSpectralAllocator()));
}

TEST(EnvironmentalLightTest, EmissionSucceeds) {
  iris::spectra::MockSpectrum spectrum;
  iris::visual_t pdf;

  auto environmental_light =
      std::make_unique<iris::environmental_lights::MockEnvironmentalLight>();
  EXPECT_CALL(*environmental_light, Emission(testing::_, testing::_, &pdf))
      .WillOnce(testing::DoAll(testing::SetArgPointee<2>(1.0),
                               testing::Return(&spectrum)));

  iris::internal::EnvironmentalLight light(
      iris::ReferenceCounted(std::move(environmental_light)));

  EXPECT_EQ(&spectrum,
            light.Emission(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                     iris::Vector(0.0, 0.0, 1.0)),
                           iris::testing::GetAlwaysVisibleVisibilityTester(),
                           iris::testing::GetSpectralAllocator(), &pdf));
  EXPECT_EQ(1.0, pdf);
}

TEST(EnvironmentalLightTest, SampleFails) {
  auto environmental_light =
      std::make_unique<iris::environmental_lights::MockEnvironmentalLight>();
  EXPECT_CALL(*environmental_light, Sample(testing::_, testing::_))
      .WillOnce(testing::Return(std::nullopt));

  iris::internal::EnvironmentalLight light(
      iris::ReferenceCounted(std::move(environmental_light)));

  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));

  EXPECT_FALSE(light.Sample(iris::Point(0.0, 0.0, 0.0), iris::Sampler(random),
                            iris::testing::GetNeverVisibleVisibilityTester(),
                            iris::testing::GetSpectralAllocator()));
}

TEST(EnvironmentalLightTest, SampleNotVisible) {
  iris::spectra::MockSpectrum spectrum;

  auto environmental_light =
      std::make_unique<iris::environmental_lights::MockEnvironmentalLight>();
  EXPECT_CALL(*environmental_light, Sample(testing::_, testing::_))
      .WillOnce(testing::Return(iris::EnvironmentalLight::SampleResult{
          spectrum, iris::Vector(1.0, 0.0, 0.0), 1.0}));

  iris::internal::EnvironmentalLight light(
      iris::ReferenceCounted(std::move(environmental_light)));

  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));

  EXPECT_FALSE(light.Sample(iris::Point(0.0, 0.0, 0.0), iris::Sampler(random),
                            iris::testing::GetNeverVisibleVisibilityTester(),
                            iris::testing::GetSpectralAllocator()));
}

TEST(EnvironmentalLightTest, SampleSucceeds) {
  iris::spectra::MockSpectrum spectrum;

  auto environmental_light =
      std::make_unique<iris::environmental_lights::MockEnvironmentalLight>();
  EXPECT_CALL(*environmental_light, Sample(testing::_, testing::_))
      .WillOnce(testing::Return(iris::EnvironmentalLight::SampleResult{
          spectrum, iris::Vector(1.0, 0.0, 0.0), 1.0}));

  iris::internal::EnvironmentalLight light(
      iris::ReferenceCounted(std::move(environmental_light)));

  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));

  auto result = light.Sample(iris::Point(0.0, 0.0, 0.0), iris::Sampler(random),
                             iris::testing::GetAlwaysVisibleVisibilityTester(),
                             iris::testing::GetSpectralAllocator());
  ASSERT_TRUE(result);
  EXPECT_EQ(&spectrum, &result->emission);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0), result->to_light);
  EXPECT_EQ(1.0, result->pdf);
}