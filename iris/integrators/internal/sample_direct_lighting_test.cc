#include "iris/integrators/internal/sample_direct_lighting.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/bxdfs/mock_bxdf.h"
#include "iris/lights/mock_light.h"
#include "iris/random/mock_random.h"
#include "iris/reflectors/mock_reflector.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/spectral_allocator.h"
#include "iris/testing/visibility_tester.h"

using iris::integrators::internal::EstimateDirectLighting;
using iris::integrators::internal::internal::DeltaLight;
using iris::integrators::internal::internal::FromBsdfSample;
using iris::integrators::internal::internal::FromLightSample;
using iris::integrators::internal::internal::PowerHeuristic;

TEST(PowerHeuristict, Test) {
  EXPECT_NEAR(0.5, PowerHeuristic(1.0, 1.0), 0.001);
  EXPECT_NEAR(0.2, PowerHeuristic(1.0, 2.0), 0.001);
}

TEST(DeltaLight, NoReflectance) {
  iris::spectra::MockSpectrum spectrum;
  iris::Vector to_light(1.0, 0.0, 0.0);

  iris::Light::SampleResult light_sample{spectrum, to_light, std::nullopt};

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(bxdf, Pdf(testing::_, testing::_, testing::_))
      .WillOnce(testing::Return(std::nullopt));

  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, to_light, to_light), iris::Point(0.0, 0.0, 0.0),
      to_light, to_light};

  EXPECT_EQ(
      nullptr,
      DeltaLight(light_sample, iris::Ray(iris::Point(0.0, 0.0, 0.0), to_light),
                 intersection, iris::testing::GetSpectralAllocator()));
}

TEST(DeltaLight, WithReflectance) {
  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));

  iris::spectra::MockSpectrum spectrum;
  EXPECT_CALL(spectrum, Intensity(testing::_)).WillOnce(testing::Return(1.0));

  iris::Vector to_light(0.0, 0.866025, 0.5);

  iris::Light::SampleResult light_sample{spectrum, to_light, std::nullopt};

  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(1.0)).WillOnce(testing::Return(0.25));

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(
      bxdf, Pdf(trace_ray.direction, to_light, iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(100.0));
  EXPECT_CALL(bxdf, Reflectance(trace_ray.direction, to_light,
                                iris::Bxdf::SampleSource::LIGHT, testing::_,
                                testing::_))
      .WillOnce(testing::Return(&reflector));

  iris::Vector surface_normal(0.0, 0.0, 1.0);
  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, surface_normal, surface_normal), trace_ray.Endpoint(1.0),
      surface_normal, surface_normal};

  auto* result = DeltaLight(light_sample, trace_ray, intersection,
                            iris::testing::GetSpectralAllocator());
  ASSERT_NE(nullptr, result);
  EXPECT_NEAR(0.125, result->Intensity(1.0), 0.001);
}

TEST(FromLightSample, NoReflectance) {
  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));

  iris::spectra::MockSpectrum spectrum;

  iris::Vector to_light(0.0, 0.866025, 0.5);

  iris::Light::SampleResult light_sample{spectrum, to_light, std::nullopt};

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(
      bxdf, Pdf(trace_ray.direction, to_light, iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(100.0));
  EXPECT_CALL(bxdf, Reflectance(trace_ray.direction, to_light,
                                iris::Bxdf::SampleSource::LIGHT, testing::_,
                                testing::_))
      .WillOnce(testing::Return(nullptr));

  iris::Vector surface_normal(0.0, 0.0, 1.0);
  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, surface_normal, surface_normal), trace_ray.Endpoint(1.0),
      surface_normal, surface_normal};

  auto* result =
      FromLightSample(light_sample, trace_ray, intersection,
                      iris::testing::GetAlwaysVisibleVisibilityTester(),
                      iris::testing::GetSpectralAllocator());
  EXPECT_EQ(nullptr, result);
}

TEST(FromLightSample, WithReflectance) {
  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));

  iris::spectra::MockSpectrum spectrum;
  EXPECT_CALL(spectrum, Intensity(testing::_)).WillOnce(testing::Return(1.0));

  iris::Vector to_light(0.0, 0.866025, 0.5);

  iris::Light::SampleResult light_sample{spectrum, to_light, 1.0};

  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(1.0)).WillOnce(testing::Return(0.5));

  iris::bxdfs::MockBxdf bxdf;
  EXPECT_CALL(
      bxdf, Pdf(trace_ray.direction, to_light, iris::Bxdf::SampleSource::LIGHT))
      .WillOnce(testing::Return(2.0));
  EXPECT_CALL(bxdf, Reflectance(trace_ray.direction, to_light,
                                iris::Bxdf::SampleSource::LIGHT, testing::_,
                                testing::_))
      .WillOnce(testing::Return(&reflector));

  iris::Vector surface_normal(0.0, 0.0, 1.0);
  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, surface_normal, surface_normal), trace_ray.Endpoint(1.0),
      surface_normal, surface_normal};

  auto* result =
      FromLightSample(light_sample, trace_ray, intersection,
                      iris::testing::GetAlwaysVisibleVisibilityTester(),
                      iris::testing::GetSpectralAllocator());
  ASSERT_NE(nullptr, result);
  EXPECT_NEAR(0.05, result->Intensity(1.0), 0.001);
}

TEST(FromBsdfSample, NoEmission) {
  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));

  iris::Vector to_light(0.0, 0.866025, 0.5);

  iris::reflectors::MockReflector reflector;

  iris::Bsdf::SampleResult bsdf_sample{reflector, to_light, 1.0};

  iris::bxdfs::MockBxdf bxdf;

  iris::Vector surface_normal(0.0, 0.0, 1.0);
  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, surface_normal, surface_normal), trace_ray.Endpoint(1.0),
      surface_normal, surface_normal};

  iris::lights::MockLight light;
  EXPECT_CALL(light, Emission(testing::_, testing::_, testing::_, testing::_))
      .WillOnce(testing::Return(nullptr));

  auto* result =
      FromBsdfSample(bsdf_sample, light, trace_ray, intersection,
                     iris::testing::GetAlwaysVisibleVisibilityTester(),
                     iris::testing::GetSpectralAllocator());
  EXPECT_EQ(nullptr, result);
}

TEST(FromBsdfSample, ZeroPdf) {
  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));

  iris::Vector to_light(0.0, 0.866025, 0.5);

  iris::reflectors::MockReflector reflector;

  iris::Bsdf::SampleResult bsdf_sample{reflector, to_light, 1.0};

  iris::bxdfs::MockBxdf bxdf;

  iris::Vector surface_normal(0.0, 0.0, 1.0);
  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, surface_normal, surface_normal), trace_ray.Endpoint(1.0),
      surface_normal, surface_normal};

  iris::lights::MockLight light;
  EXPECT_CALL(light, Emission(testing::_, testing::_, testing::_, testing::_))
      .WillOnce(testing::DoAll(testing::SetArgPointee<3, iris::visual_t>(0.0),
                               testing::Return(nullptr)));

  auto* result =
      FromBsdfSample(bsdf_sample, light, trace_ray, intersection,
                     iris::testing::GetAlwaysVisibleVisibilityTester(),
                     iris::testing::GetSpectralAllocator());
  EXPECT_EQ(nullptr, result);
}

TEST(FromBsdfSample, NegativePdf) {
  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));

  iris::Vector to_light(0.0, 0.866025, 0.5);

  iris::reflectors::MockReflector reflector;

  iris::Bsdf::SampleResult bsdf_sample{reflector, to_light, 1.0};

  iris::bxdfs::MockBxdf bxdf;

  iris::Vector surface_normal(0.0, 0.0, 1.0);
  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, surface_normal, surface_normal), trace_ray.Endpoint(1.0),
      surface_normal, surface_normal};

  iris::lights::MockLight light;
  EXPECT_CALL(light, Emission(testing::_, testing::_, testing::_, testing::_))
      .WillOnce(testing::DoAll(testing::SetArgPointee<3, iris::visual_t>(-1.0),
                               testing::Return(nullptr)));

  auto* result =
      FromBsdfSample(bsdf_sample, light, trace_ray, intersection,
                     iris::testing::GetAlwaysVisibleVisibilityTester(),
                     iris::testing::GetSpectralAllocator());
  EXPECT_EQ(nullptr, result);
}

TEST(FromBsdfSample, WithEmission) {
  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));

  iris::Vector to_light(0.0, 0.866025, 0.5);

  iris::reflectors::MockReflector reflector;
  EXPECT_CALL(reflector, Reflectance(1.0)).WillOnce(testing::Return(0.5));

  iris::Bsdf::SampleResult bsdf_sample{reflector, to_light, 1.0};

  iris::bxdfs::MockBxdf bxdf;

  iris::Vector surface_normal(0.0, 0.0, 1.0);
  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, surface_normal, surface_normal), trace_ray.Endpoint(1.0),
      surface_normal, surface_normal};

  iris::spectra::MockSpectrum spectrum;
  EXPECT_CALL(spectrum, Intensity(testing::_)).WillOnce(testing::Return(1.0));

  iris::lights::MockLight light;
  EXPECT_CALL(light, Emission(iris::Ray(trace_ray.Endpoint(1.0), to_light),
                              testing::_, testing::_, testing::_))
      .WillOnce(testing::DoAll(testing::SetArgPointee<3, iris::visual_t>(2.0),
                               testing::Return(&spectrum)));

  auto* result =
      FromBsdfSample(bsdf_sample, light, trace_ray, intersection,
                     iris::testing::GetAlwaysVisibleVisibilityTester(),
                     iris::testing::GetSpectralAllocator());
  ASSERT_NE(nullptr, result);
  EXPECT_NEAR(0.05, result->Intensity(1.0), 0.001);
}

TEST(EstimateDirectLighting, NoSamples) {
  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));
  auto hit_point = trace_ray.Endpoint(1.0);
  iris::Vector surface_normal(0.0, 0.0, 1.0);
  iris::bxdfs::MockBxdf bxdf;
  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, surface_normal, surface_normal), hit_point,
      surface_normal, surface_normal};

  iris::Vector to_light(0.0, 0.866025, 0.5);

  EXPECT_CALL(bxdf, Sample(trace_ray.direction, testing::_))
      .WillOnce(testing::Return(to_light));
  EXPECT_CALL(bxdf, Pdf(trace_ray.direction, to_light, testing::_))
      .WillOnce(testing::Return(0.0));

  iris::lights::MockLight light;
  EXPECT_CALL(light, Sample(hit_point, testing::_, testing::_, testing::_))
      .WillOnce(testing::Return(std::nullopt));

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, DiscardGeometric(2)).Times(2);

  auto* result = EstimateDirectLighting(
      light, trace_ray, intersection, iris::Sampler(rng), iris::Sampler(rng),
      iris::testing::GetAlwaysVisibleVisibilityTester(),
      iris::testing::GetSpectralAllocator());
  ASSERT_EQ(nullptr, result);
}

TEST(EstimateDirectLighting, DeltaLight) {
  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));
  auto hit_point = trace_ray.Endpoint(1.0);
  iris::Vector surface_normal(0.0, 0.0, 1.0);
  iris::bxdfs::MockBxdf bxdf;
  iris::RayTracer::RayTracer::SurfaceIntersection intersection{
      iris::Bsdf(bxdf, surface_normal, surface_normal), hit_point,
      surface_normal, surface_normal};

  iris::Vector to_light(0.0, 0.866025, 0.5);

  iris::reflectors::MockReflector reflector;

  EXPECT_CALL(bxdf, Sample(trace_ray.direction, testing::_))
      .WillOnce(testing::Return(to_light));
  EXPECT_CALL(bxdf, Pdf(trace_ray.direction, to_light, testing::_))
      .WillRepeatedly(testing::Return(1.0));
  EXPECT_CALL(bxdf, Reflectance(trace_ray.direction, to_light, testing::_,
                                testing::_, testing::_))
      .WillRepeatedly(testing::Return(&reflector));

  iris::spectra::MockSpectrum spectrum;
  iris::Light::SampleResult light_sample{spectrum, to_light, std::nullopt};

  iris::lights::MockLight light;
  EXPECT_CALL(light, Sample(hit_point, testing::_, testing::_, testing::_))
      .WillOnce(testing::Return(light_sample));

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, DiscardGeometric(2)).Times(2);

  auto* result = EstimateDirectLighting(
      light, trace_ray, intersection, iris::Sampler(rng), iris::Sampler(rng),
      iris::testing::GetAlwaysVisibleVisibilityTester(),
      iris::testing::GetSpectralAllocator());
  ASSERT_NE(nullptr, result);

  EXPECT_CALL(reflector, Reflectance(1.0)).WillOnce(testing::Return(0.25));
  EXPECT_CALL(spectrum, Intensity(testing::_)).WillOnce(testing::Return(1.0));
  EXPECT_NEAR(0.125, result->Intensity(1.0), 0.001);
}