#include "iris/internal/area_light.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/emissive_materials/mock_emissive_material.h"
#include "iris/random/mock_random.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/spectral_allocator.h"
#include "iris/testing/visibility_tester.h"

class TestLightedGeometry : public iris::Geometry {
 public:
  TestLightedGeometry(iris::Point location, iris::geometric distance,
                      const iris::EmissiveMaterial* emissive_material,
                      bool allow_sampling = true)
      : location_(location),
        distance_(distance),
        emissive_material_(emissive_material),
        allow_sampling_(allow_sampling) {}

  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const override {
    return &hit_allocator.Allocate(nullptr, distance_, 1u, 2u);
  }

  bool IsEmissive(iris::face_t face) const override { return face == 1u; }

  const iris::EmissiveMaterial* GetEmissiveMaterial(
      iris::face_t face, const void* additional_data) const override {
    return emissive_material_;
  }

  std::optional<iris::Point> SampleFace(iris::face_t face,
                                        iris::Sampler& sampler) const override {
    if (allow_sampling_) {
      return location_;
    }
    return std::nullopt;
  }

  iris::Vector ComputeSurfaceNormal(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const override {
    return iris::Vector(0.0, 0.0, std::copysign(1.0, -location_.z));
  }

  std::optional<iris::visual_t> ComputeArea(iris::face_t face) const override {
    return 1.0;
  }

  std::span<const iris::face_t> GetFaces() const override {
    static const iris::face_t faces[] = {1u, 2u};
    return faces;
  }

  iris::Point location_;
  iris::geometric distance_;
  const iris::EmissiveMaterial* emissive_material_;
  bool allow_sampling_;
};

TEST(AreaLightTest, AreaLightEmission) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;
  EXPECT_CALL(emissive_material, Evaluate(testing::_, testing::_))
      .WillOnce(testing::Return(&spectrum));

  TestLightedGeometry geometry(iris::Point(0.0, 0.0, 0.0), 1.0,
                               &emissive_material);
  iris::internal::AreaLight light(geometry, nullptr, 1);

  iris::testing::ScopedSingleGeometryVisibilityTester(
      geometry, nullptr, [&](auto& visibility_tester) {
        EXPECT_EQ(&spectrum,
                  light.Emission(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                           iris::Vector(0.0, 0.0, 1.0)),
                                 visibility_tester,
                                 iris::testing::GetSpectralAllocator()));
      });
}

TEST(AreaLightTest, AreaLightEmissionMisses) {
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  TestLightedGeometry geometry(iris::Point(0.0, 0.0, 0.0), 1.0,
                               &emissive_material);
  iris::internal::AreaLight light(geometry, nullptr, 1);

  EXPECT_EQ(nullptr,
            light.Emission(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                     iris::Vector(0.0, 0.0, 1.0)),
                           iris::testing::GetNeverVisibleVisibilityTester(),
                           iris::testing::GetSpectralAllocator()));
}

TEST(AreaLightTest, AreaLightSampleRngFails) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  TestLightedGeometry geometry(iris::Point(0.0, 0.0, 0.0), 1.0,
                               &emissive_material, false);
  iris::internal::AreaLight light(geometry, nullptr, 1);

  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));

  iris::testing::ScopedSingleGeometryVisibilityTester(
      geometry, nullptr, [&](auto& visibility_tester) {
        EXPECT_FALSE(light.Sample(iris::Point(0.0, 0.0, 0.0),
                                  iris::Sampler(random), visibility_tester,
                                  iris::testing::GetSpectralAllocator()));
      });
}

TEST(AreaLightTest, AreaLightSampleNotVisible) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  TestLightedGeometry geometry(iris::Point(0.0, 0.0, 1.0), 1.0,
                               &emissive_material);
  iris::internal::AreaLight light(geometry, nullptr, 1);

  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));

  EXPECT_FALSE(light.Sample(iris::Point(0.0, 0.0, 0.0), iris::Sampler(random),
                            iris::testing::GetNeverVisibleVisibilityTester(),
                            iris::testing::GetSpectralAllocator()));
}

TEST(AreaLightTest, AreaLightSampleWorld) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  EXPECT_CALL(emissive_material, Evaluate(testing::_, testing::_))
      .WillOnce(testing::Return(&spectrum));

  TestLightedGeometry geometry(iris::Point(0.0, 0.0, 1.0), 1.0,
                               &emissive_material);
  iris::internal::AreaLight light(geometry, nullptr, 1);

  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));

  iris::testing::ScopedSingleGeometryVisibilityTester(
      geometry, nullptr, [&](auto& visibility_tester) {
        auto result = light.Sample(iris::Point(0.0, 0.0, 0.0),
                                   iris::Sampler(random), visibility_tester,
                                   iris::testing::GetSpectralAllocator());
        EXPECT_TRUE(result);
        EXPECT_EQ(&spectrum, &result->emission);
        EXPECT_EQ(1.0, result->pdf);
        EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result->to_light);
      });
}

TEST(AreaLightTest, AreaLightSampleWithTransform) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  EXPECT_CALL(emissive_material, Evaluate(testing::_, testing::_))
      .WillOnce(testing::Return(&spectrum));

  auto transform = iris::Matrix::Scalar(1.0, 1.0, -1.0).value();
  TestLightedGeometry geometry(iris::Point(0.0, 0.0, -1.0), 1.0,
                               &emissive_material);
  iris::internal::AreaLight light(geometry, &transform, 1);

  iris::random::MockRandom random;
  EXPECT_CALL(random, DiscardGeometric(2));

  iris::testing::ScopedSingleGeometryVisibilityTester(
      geometry, &transform, [&](auto& visibility_tester) {
        auto result = light.Sample(iris::Point(0.0, 0.0, 0.0),
                                   iris::Sampler(random), visibility_tester,
                                   iris::testing::GetSpectralAllocator());
        EXPECT_TRUE(result);
        EXPECT_EQ(&spectrum, &result->emission);
        EXPECT_EQ(1.0, result->pdf);
        EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result->to_light);
      });
}