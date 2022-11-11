#include "iris/integrators/path_integrator.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/environmental_lights/mock_environmental_light.h"
#include "iris/random/mock_random.h"
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