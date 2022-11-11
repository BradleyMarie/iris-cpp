#include "iris/integrators/path_integrator.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/bxdfs/mock_bxdf.h"
#include "iris/environmental_lights/mock_environmental_light.h"
#include "iris/random/mock_random.h"
#include "iris/reflectors/mock_reflector.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/light_sampler.h"
#include "iris/testing/ray_tracer.h"
#include "iris/testing/spectral_allocator.h"
#include "iris/testing/visibility_tester.h"

TEST(PathIntegratorTest, NoHits) {
  iris::random::MockRandom rng;

  iris::integrators::PathIntegrator integrator(1.0, 1.0, 1u, 8u);
  EXPECT_EQ(nullptr, integrator.Integrate(
                         iris::Ray(iris::Point(0.0, 0.0, 1.0),
                                   iris::Vector(0.0, 0.0, 1.0)),
                         iris::testing::GetNoHitsRayTracer(),
                         iris::testing::GetEmptyLightSampler(),
                         iris::testing::GetAlwaysVisibleVisibilityTester(),
                         iris::testing::GetSpectralAllocator(), rng));
}

TEST(PathIntegratorTest, HitsEmissiveWithNoReflectance) {
  iris::spectra::MockSpectrum spectrum;
  iris::environmental_lights::MockEnvironmentalLight environmental_light;
  EXPECT_CALL(environmental_light, Emission(testing::_, testing::_, testing::_))
      .WillOnce(testing::Return(&spectrum));
  iris::random::MockRandom rng;

  iris::integrators::PathIntegrator integrator(1.0, 1.0, 1u, 8u);
  iris::testing::ScopedNoHitsRayTracer(
      environmental_light, [&](auto& ray_tracer) {
        EXPECT_EQ(&spectrum,
                  integrator.Integrate(
                      iris::Ray(iris::Point(0.0, 0.0, 1.0),
                                iris::Vector(0.0, 0.0, 1.0)),
                      ray_tracer, iris::testing::GetEmptyLightSampler(),
                      iris::testing::GetAlwaysVisibleVisibilityTester(),
                      iris::testing::GetSpectralAllocator(), rng));
      });
}

TEST(PathIntegratorTest, BounceLimit) {
  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(testing::_))
      .WillRepeatedly(testing::Return(0.5));
  EXPECT_CALL(reflector, Albedo()).WillRepeatedly(testing::Return(0.5));

  iris::bxdfs::MockBxdf specular0;
  EXPECT_CALL(specular0, Sample(testing::_, testing::_))
      .WillRepeatedly(testing::Return(iris::Vector(0.0, 0.0, -1.0)));
  EXPECT_CALL(specular0, Pdf(testing::_, testing::_, testing::_))
      .WillRepeatedly(testing::Return(absl::nullopt));
  EXPECT_CALL(specular0, Reflectance(testing::_, testing::_, testing::_,
                                     testing::_, testing::_))
      .WillRepeatedly(testing::Return(&reflector));

  iris::spectra::MockSpectrum spectrum;
  EXPECT_CALL(spectrum, Intensity(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, DiscardGeometric(2)).Times(1);

  iris::testing::RayTracerPathNode path[] = {
      {1.0, nullptr, &specular0, iris::Vector(0.0, 0.0, -1.0),
       iris::Vector(0.0, 0.0, -1.0)},
      {1.0, &spectrum, nullptr, iris::Vector(0.0, 0.0, 1.0),
       iris::Vector(0.0, 0.0, 1.0)}};

  iris::integrators::PathIntegrator integrator0(1.0, 1.0, 0u, 0u);
  iris::testing::ScopedHitsRayTracer(nullptr, path, [&](auto& ray_tracer) {
    EXPECT_EQ(nullptr, integrator0.Integrate(
                           iris::Ray(iris::Point(0.0, 0.0, 1.0),
                                     iris::Vector(0.0, 0.0, 1.0)),
                           ray_tracer, iris::testing::GetEmptyLightSampler(),
                           iris::testing::GetAlwaysVisibleVisibilityTester(),
                           iris::testing::GetSpectralAllocator(), rng));
  });

  iris::integrators::PathIntegrator integrator1(1.0, 1.0, 1u, 1u);
  iris::testing::ScopedHitsRayTracer(nullptr, path, [&](auto& ray_tracer) {
    auto* result = integrator1.Integrate(
        iris::Ray(iris::Point(0.0, 0.0, 1.0), iris::Vector(0.0, 0.0, 1.0)),
        ray_tracer, iris::testing::GetEmptyLightSampler(),
        iris::testing::GetAlwaysVisibleVisibilityTester(),
        iris::testing::GetSpectralAllocator(), rng);
    EXPECT_TRUE(result);
    EXPECT_EQ(0.5, result->Intensity(1.0));
  });
}

TEST(PathIntegratorTest, NoBsdf) {
  iris::spectra::MockSpectrum spectrum;
  EXPECT_CALL(spectrum, Intensity(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::random::MockRandom rng;

  iris::testing::RayTracerPathNode path[] = {
      {1.0, nullptr, nullptr, iris::Vector(0.0, 0.0, -1.0),
       iris::Vector(0.0, 0.0, -1.0)},
      {1.0, &spectrum, nullptr, iris::Vector(0.0, 0.0, 1.0),
       iris::Vector(0.0, 0.0, 1.0)}};

  iris::integrators::PathIntegrator integrator(1.0, 1.0, 3u, 3u);
  iris::testing::ScopedHitsRayTracer(nullptr, path, [&](auto& ray_tracer) {
    EXPECT_EQ(nullptr, integrator.Integrate(
                           iris::Ray(iris::Point(0.0, 0.0, 1.0),
                                     iris::Vector(0.0, 0.0, 1.0)),
                           ray_tracer, iris::testing::GetEmptyLightSampler(),
                           iris::testing::GetAlwaysVisibleVisibilityTester(),
                           iris::testing::GetSpectralAllocator(), rng));
  });
}

TEST(PathIntegratorTest, BsdfSampleFails) {
  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(testing::_))
      .WillRepeatedly(testing::Return(0.5));
  EXPECT_CALL(reflector, Albedo()).WillRepeatedly(testing::Return(0.5));

  iris::bxdfs::MockBxdf specular0;
  EXPECT_CALL(specular0, Sample(testing::_, testing::_))
      .WillRepeatedly(testing::Return(iris::Vector(0.0, 0.0, -1.0)));
  EXPECT_CALL(specular0, Pdf(testing::_, testing::_, testing::_))
      .WillRepeatedly(testing::Return(0.0));

  iris::spectra::MockSpectrum spectrum;
  EXPECT_CALL(spectrum, Intensity(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, DiscardGeometric(2)).Times(1);

  iris::testing::RayTracerPathNode path[] = {
      {1.0, nullptr, &specular0, iris::Vector(0.0, 0.0, -1.0),
       iris::Vector(0.0, 0.0, -1.0)},
      {1.0, &spectrum, nullptr, iris::Vector(0.0, 0.0, 1.0),
       iris::Vector(0.0, 0.0, 1.0)}};

  iris::integrators::PathIntegrator integrator0(1.0, 1.0, 3u, 3u);
  iris::testing::ScopedHitsRayTracer(nullptr, path, [&](auto& ray_tracer) {
    EXPECT_EQ(nullptr, integrator0.Integrate(
                           iris::Ray(iris::Point(0.0, 0.0, 1.0),
                                     iris::Vector(0.0, 0.0, 1.0)),
                           ray_tracer, iris::testing::GetEmptyLightSampler(),
                           iris::testing::GetAlwaysVisibleVisibilityTester(),
                           iris::testing::GetSpectralAllocator(), rng));
  });
}

TEST(PathIntegratorTest, TwoSpecularBouncesHitsEmissive) {
  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(testing::_))
      .WillRepeatedly(testing::Return(0.5));
  EXPECT_CALL(reflector, Albedo()).WillRepeatedly(testing::Return(0.5));

  iris::bxdfs::MockBxdf specular0, specular1;
  EXPECT_CALL(specular0, Sample(testing::_, testing::_))
      .WillRepeatedly(testing::Return(iris::Vector(0.0, 0.0, -1.0)));
  EXPECT_CALL(specular1, Sample(testing::_, testing::_))
      .WillRepeatedly(testing::Return(iris::Vector(0.0, 0.0, 1.0)));
  EXPECT_CALL(specular0, Pdf(testing::_, testing::_, testing::_))
      .WillRepeatedly(testing::Return(absl::nullopt));
  EXPECT_CALL(specular1, Pdf(testing::_, testing::_, testing::_))
      .WillRepeatedly(testing::Return(absl::nullopt));
  EXPECT_CALL(specular0, Reflectance(testing::_, testing::_, testing::_,
                                     testing::_, testing::_))
      .WillRepeatedly(testing::Return(&reflector));
  EXPECT_CALL(specular1, Reflectance(testing::_, testing::_, testing::_,
                                     testing::_, testing::_))
      .WillRepeatedly(testing::Return(&reflector));

  iris::spectra::MockSpectrum spectrum;
  EXPECT_CALL(spectrum, Intensity(testing::_))
      .WillRepeatedly(testing::Return(1.0));

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, DiscardGeometric(2)).Times(2);

  iris::testing::RayTracerPathNode path[] = {
      {1.0, nullptr, &specular0, iris::Vector(0.0, 0.0, -1.0),
       iris::Vector(0.0, 0.0, -1.0)},
      {1.0, nullptr, &specular1, iris::Vector(0.0, 0.0, 1.0),
       iris::Vector(0.0, 0.0, 1.0)},
      {1.0, &spectrum, nullptr, iris::Vector(0.0, 0.0, -1.0),
       iris::Vector(0.0, 0.0, -1.0)}};

  iris::integrators::PathIntegrator integrator(1.0, 1.0, 4u, 8u);
  iris::testing::ScopedHitsRayTracer(nullptr, path, [&](auto& ray_tracer) {
    auto* result = integrator.Integrate(
        iris::Ray(iris::Point(0.0, 0.0, 1.0), iris::Vector(0.0, 0.0, 1.0)),
        ray_tracer, iris::testing::GetEmptyLightSampler(),
        iris::testing::GetAlwaysVisibleVisibilityTester(),
        iris::testing::GetSpectralAllocator(), rng);
    EXPECT_TRUE(result);
    EXPECT_EQ(0.25, result->Intensity(1.0));
  });
}