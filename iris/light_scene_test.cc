#include "iris/light_scene.h"

#include <memory>
#include <optional>
#include <vector>

#include "googletest/include/gtest/gtest.h"
#include "iris/emissive/mock_emissive_material.h"
#include "iris/lights/point_light.h"
#include "iris/random/mock_random.h"
#include "iris/scenes/list_scene.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/light_tester.h"

class TestLightSceneBuilder final : public iris::LightScene::Builder {
 public:
  TestLightSceneBuilder(
      size_t num_expected_lights,
      std::optional<std::vector<iris::Light*>> expected_lights = std::nullopt)
      : num_expected_lights_(num_expected_lights),
        expected_lights_(expected_lights) {}

  std::unique_ptr<iris::LightScene> Build(
      std::vector<std::unique_ptr<iris::Light>> lights) override;

  std::unique_ptr<iris::Light> light_;

 private:
  size_t num_expected_lights_;
  std::optional<std::vector<iris::Light*>> expected_lights_;
};

std::unique_ptr<iris::LightScene> TestLightSceneBuilder::Build(
    std::vector<std::unique_ptr<iris::Light>> lights) {
  EXPECT_EQ(num_expected_lights_, lights.size());
  if (expected_lights_) {
    EXPECT_EQ(expected_lights_->size(), lights.size());
    for (size_t i = 0u; i < expected_lights_->size(); i++) {
      EXPECT_EQ(expected_lights_->at(i), lights.at(i).get());
    }
  }

  if (!lights.empty()) {
    light_ = std::move(lights.front());
  }

  return nullptr;
}

class TestGeometry : public iris::Geometry {
 public:
  TestGeometry(iris::Point location, iris::geometric_t distance,
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

  bool IsEmissive(iris::face_t face) const override { return true; }

  const iris::EmissiveMaterial* GetEmissiveMaterial(
      iris::face_t face, const void* additional_data) const override {
    return emissive_material_;
  }

  std::optional<iris::Point> SampleFace(iris::face_t face,
                                        iris::Random& rng) const override {
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
  iris::geometric_t distance_;
  const iris::EmissiveMaterial* emissive_material_;
  bool allow_sampling_;
};

TEST(ListSceneTest, BuilderEmpty) {
  auto scene = iris::scenes::ListScene::Builder::Create()->Build();
  std::unique_ptr<iris::LightScene::Builder> builder =
      std::make_unique<TestLightSceneBuilder>(0);
  ASSERT_EQ(nullptr, builder->Build(*scene).get());
}

TEST(ListSceneTest, BuilderOneLight) {
  auto test_light = std::make_unique<iris::lights::PointLight>(
      nullptr, iris::Point(1.0, 1.0, 1.0));
  auto scene = iris::scenes::ListScene::Builder::Create()->Build();
  std::unique_ptr<iris::LightScene::Builder> builder =
      std::make_unique<TestLightSceneBuilder>(
          1, std::vector<iris::Light*>({test_light.get()}));
  builder->Add(std::move(test_light));
  ASSERT_EQ(nullptr, builder->Build(*scene).get());
}

TEST(ListSceneTest, BuilderOneAreaLight) {
  auto scene_builder = iris::scenes::ListScene::Builder::Create();
  scene_builder->Add(
      std::make_unique<TestGeometry>(iris::Point(0.0, 0.0, 0.0), 1.0, nullptr));
  auto scene = scene_builder->Build();

  std::unique_ptr<iris::LightScene::Builder> builder =
      std::make_unique<TestLightSceneBuilder>(2);
  ASSERT_EQ(nullptr, builder->Build(*scene).get());
}

TEST(ListSceneTest, AreaLightEmission) {
  iris::testing::LightTester light_tester;
  iris::spectra::MockSpectrum spectrum;
  iris::emissive::MockEmissiveMaterial emissive_material;
  EXPECT_CALL(emissive_material, Compute(testing::_))
      .Times(1)
      .WillOnce(testing::Return(&spectrum));

  auto scene_builder = iris::scenes::ListScene::Builder::Create();
  scene_builder->Add(std::make_unique<TestGeometry>(iris::Point(0.0, 0.0, 0.0),
                                                    1.0, &emissive_material));
  auto scene = scene_builder->Build();

  auto builder = std::make_unique<TestLightSceneBuilder>(2);
  ASSERT_EQ(nullptr, static_cast<iris::LightScene::Builder*>(builder.get())
                         ->Build(*scene)
                         .get());

  EXPECT_EQ(&spectrum,
            light_tester.Emission(*builder->light_,
                                  iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                            iris::Vector(0.0, 0.0, 1.0)),
                                  *scene));
}

TEST(ListSceneTest, AreaLightEmissionMisses) {
  iris::testing::LightTester light_tester;
  iris::spectra::MockSpectrum spectrum;
  iris::emissive::MockEmissiveMaterial emissive_material;

  auto scene_builder = iris::scenes::ListScene::Builder::Create();
  scene_builder->Add(std::make_unique<TestGeometry>(iris::Point(0.0, 0.0, 0.0),
                                                    1.0, &emissive_material));
  auto scene = scene_builder->Build();

  auto builder = std::make_unique<TestLightSceneBuilder>(2);
  ASSERT_EQ(nullptr, static_cast<iris::LightScene::Builder*>(builder.get())
                         ->Build(*scene)
                         .get());

  EXPECT_EQ(nullptr, light_tester.Emission(
                         *builder->light_,
                         iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                   iris::Vector(0.0, 0.0, 1.0)),
                         *iris::scenes::ListScene::Builder::Create()->Build()));
}

TEST(ListSceneTest, AreaLightSampleRngFails) {
  iris::testing::LightTester light_tester;
  iris::spectra::MockSpectrum spectrum;
  iris::emissive::MockEmissiveMaterial emissive_material;

  auto scene_builder = iris::scenes::ListScene::Builder::Create();
  scene_builder->Add(std::make_unique<TestGeometry>(
      iris::Point(0.0, 0.0, 0.0), 1.0, &emissive_material, false));
  auto scene = scene_builder->Build();

  auto builder = std::make_unique<TestLightSceneBuilder>(2);
  ASSERT_EQ(nullptr, static_cast<iris::LightScene::Builder*>(builder.get())
                         ->Build(*scene)
                         .get());

  iris::random::MockRandom random;
  EXPECT_FALSE(light_tester.Sample(*builder->light_, iris::Point(0.0, 0.0, 0.0),
                                   random, *scene));
}

TEST(ListSceneTest, AreaLightSampleNotVisible) {
  iris::testing::LightTester light_tester;
  iris::spectra::MockSpectrum spectrum;
  iris::emissive::MockEmissiveMaterial emissive_material;

  auto scene_builder = iris::scenes::ListScene::Builder::Create();
  scene_builder->Add(std::make_unique<TestGeometry>(iris::Point(0.0, 0.0, 1.0),
                                                    1.0, &emissive_material));
  auto scene = scene_builder->Build();

  auto builder = std::make_unique<TestLightSceneBuilder>(2);
  ASSERT_EQ(nullptr, static_cast<iris::LightScene::Builder*>(builder.get())
                         ->Build(*scene)
                         .get());

  iris::random::MockRandom random;
  EXPECT_FALSE(light_tester.Sample(
      *builder->light_, iris::Point(0.0, 0.0, 0.0), random,
      *iris::scenes::ListScene::Builder::Create()->Build()));
}

TEST(ListSceneTest, AreaLightSampleWorld) {
  iris::testing::LightTester light_tester;
  iris::spectra::MockSpectrum spectrum;
  iris::emissive::MockEmissiveMaterial emissive_material;

  EXPECT_CALL(emissive_material, Compute(testing::_))
      .Times(1)
      .WillOnce(testing::Return(&spectrum));

  auto scene_builder = iris::scenes::ListScene::Builder::Create();
  scene_builder->Add(std::make_unique<TestGeometry>(iris::Point(0.0, 0.0, 1.0),
                                                    1.0, &emissive_material));
  auto scene = scene_builder->Build();

  auto builder = std::make_unique<TestLightSceneBuilder>(2);
  ASSERT_EQ(nullptr, static_cast<iris::LightScene::Builder*>(builder.get())
                         ->Build(*scene)
                         .get());

  iris::random::MockRandom random;
  auto result = light_tester.Sample(*builder->light_,
                                    iris::Point(0.0, 0.0, 0.0), random, *scene);
  EXPECT_TRUE(result);
  EXPECT_EQ(&spectrum, &result->emission);
  EXPECT_EQ(1.0, result->pdf);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result->to_light);
}

TEST(ListSceneTest, AreaLightSampleWithTransform) {
  iris::testing::LightTester light_tester;
  iris::spectra::MockSpectrum spectrum;
  iris::emissive::MockEmissiveMaterial emissive_material;

  EXPECT_CALL(emissive_material, Compute(testing::_))
      .Times(1)
      .WillOnce(testing::Return(&spectrum));

  auto scene_builder = iris::scenes::ListScene::Builder::Create();
  scene_builder->Add(std::make_unique<TestGeometry>(iris::Point(0.0, 0.0, -1.0),
                                                    1.0, &emissive_material),
                     iris::Matrix::Scalar(1.0, 1.0, -1.0).value());
  auto scene = scene_builder->Build();

  auto builder = std::make_unique<TestLightSceneBuilder>(2);
  ASSERT_EQ(nullptr, static_cast<iris::LightScene::Builder*>(builder.get())
                         ->Build(*scene)
                         .get());

  iris::random::MockRandom random;
  auto result = light_tester.Sample(*builder->light_,
                                    iris::Point(0.0, 0.0, 0.0), random, *scene);
  EXPECT_TRUE(result);
  EXPECT_EQ(&spectrum, &result->emission);
  EXPECT_EQ(1.0, result->pdf);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result->to_light);
}