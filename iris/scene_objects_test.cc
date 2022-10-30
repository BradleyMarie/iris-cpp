#include "iris/scene_objects.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/emissive_materials/mock_emissive_material.h"
#include "iris/lights/mock_light.h"
#include "iris/random/mock_random.h"
#include "iris/scenes/list_scene.h"
#include "iris/spectra/mock_spectrum.h"
#include "iris/testing/spectral_allocator.h"
#include "iris/testing/visibility_tester.h"

class TestGeometry : public iris::Geometry {
 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const override {
    EXPECT_FALSE(true);
    return nullptr;
  }

 public:
  iris::Vector ComputeSurfaceNormal(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const override {
    EXPECT_FALSE(true);
    return iris::Vector(1.0, 0.0, 0.0);
  }

  std::span<const iris::face_t> GetFaces() const override {
    static const iris::face_t faces[] = {1};
    return faces;
  }
};

TEST(SceneObjects, Build) {
  auto geom0 = iris::MakeReferenceCounted<TestGeometry>();
  auto geom1 = iris::MakeReferenceCounted<TestGeometry>();
  auto geom2 = iris::MakeReferenceCounted<TestGeometry>();
  auto geom3 = iris::MakeReferenceCounted<TestGeometry>();
  auto light0 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  auto light1 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  auto matrix0 = iris::Matrix::Identity();
  auto matrix1 = iris::Matrix::Translation(1.0, 2.0, 3.0).value();
  auto matrix2 = iris::Matrix::Scalar(1.0, 2.0, 3.0).value();

  auto geom0_ptr = geom0.Get();
  auto geom1_ptr = geom1.Get();
  auto geom2_ptr = geom2.Get();
  auto geom3_ptr = geom3.Get();
  auto light0_ptr = light0.Get();
  auto light1_ptr = light1.Get();

  iris::SceneObjects::Builder builder;

  builder.Add(iris::ReferenceCounted<iris::Geometry>(), matrix0);
  builder.Add(iris::ReferenceCounted<iris::lights::MockLight>());

  builder.Add(geom0, matrix1);
  builder.Add(std::move(geom1), matrix2);
  builder.Add(std::move(geom2), matrix2);
  builder.Add(std::move(geom3), matrix0);
  builder.Add(light0);
  builder.Add(light1);

  auto scene_objects = builder.Build();
  ASSERT_EQ(4u, scene_objects.NumGeometry());
  EXPECT_EQ(geom0_ptr, &scene_objects.GetGeometry(0).first);
  EXPECT_EQ(matrix1, *scene_objects.GetGeometry(0).second);
  EXPECT_EQ(geom1_ptr, &scene_objects.GetGeometry(1).first);
  EXPECT_EQ(matrix2, *scene_objects.GetGeometry(1).second);
  EXPECT_EQ(geom2_ptr, &scene_objects.GetGeometry(2).first);
  EXPECT_EQ(matrix2, *scene_objects.GetGeometry(2).second);
  EXPECT_EQ(geom3_ptr, &scene_objects.GetGeometry(3).first);
  EXPECT_EQ(nullptr, scene_objects.GetGeometry(3).second);
  EXPECT_EQ(*scene_objects.GetGeometry(1).second,
            *scene_objects.GetGeometry(2).second);
  ASSERT_EQ(2u, scene_objects.NumLights());
  EXPECT_EQ(light0_ptr, &scene_objects.GetLight(0));
  EXPECT_EQ(light1_ptr, &scene_objects.GetLight(1));

  // Reuse builder
  geom0 = iris::MakeReferenceCounted<TestGeometry>();
  geom1 = iris::MakeReferenceCounted<TestGeometry>();
  geom2 = iris::MakeReferenceCounted<TestGeometry>();
  geom3 = iris::MakeReferenceCounted<TestGeometry>();
  light0 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  light1 = iris::MakeReferenceCounted<iris::lights::MockLight>();

  geom0_ptr = geom0.Get();
  geom1_ptr = geom1.Get();
  geom2_ptr = geom2.Get();
  geom3_ptr = geom3.Get();
  light0_ptr = light0.Get();
  light1_ptr = light1.Get();

  builder.Add(std::move(geom0), matrix1);
  builder.Add(std::move(geom1), matrix2);
  builder.Add(std::move(geom2), matrix2);
  builder.Add(std::move(geom3), matrix0);
  builder.Add(light0);
  builder.Add(light1);

  scene_objects = builder.Build();
  ASSERT_EQ(4u, scene_objects.NumGeometry());
  EXPECT_EQ(geom0_ptr, &scene_objects.GetGeometry(0).first);
  EXPECT_EQ(matrix1, *scene_objects.GetGeometry(0).second);
  EXPECT_EQ(geom1_ptr, &scene_objects.GetGeometry(1).first);
  EXPECT_EQ(matrix2, *scene_objects.GetGeometry(1).second);
  EXPECT_EQ(geom2_ptr, &scene_objects.GetGeometry(2).first);
  EXPECT_EQ(matrix2, *scene_objects.GetGeometry(2).second);
  EXPECT_EQ(geom3_ptr, &scene_objects.GetGeometry(3).first);
  EXPECT_EQ(nullptr, scene_objects.GetGeometry(3).second);
  EXPECT_EQ(*scene_objects.GetGeometry(1).second,
            *scene_objects.GetGeometry(2).second);
  ASSERT_EQ(2u, scene_objects.NumLights());
  EXPECT_EQ(light0_ptr, &scene_objects.GetLight(0));
  EXPECT_EQ(light1_ptr, &scene_objects.GetLight(1));
}

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
  iris::geometric distance_;
  const iris::EmissiveMaterial* emissive_material_;
  bool allow_sampling_;
};

TEST(ListSceneTest, BuilderOneAreaLight) {
  iris::SceneObjects::Builder builder;
  builder.Add(iris::MakeReferenceCounted<TestLightedGeometry>(
      iris::Point(0.0, 0.0, 0.0), 1.0, nullptr));
  auto scene_objects = builder.Build();
  EXPECT_EQ(1u, scene_objects.NumLights());
}

TEST(ListSceneTest, AreaLightEmission) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;
  EXPECT_CALL(emissive_material, Evaluate(testing::_, testing::_))
      .WillOnce(testing::Return(&spectrum));

  iris::SceneObjects::Builder builder;
  builder.Add(iris::MakeReferenceCounted<TestLightedGeometry>(
      iris::Point(0.0, 0.0, 0.0), 1.0, &emissive_material));
  auto scene_objects = builder.Build();

  EXPECT_EQ(1u, scene_objects.NumLights());
  const auto& light = scene_objects.GetLight(0);

  auto scene = iris::scenes::ListScene::Builder::Create()->Build(scene_objects);
  auto visibility_tester = iris::testing::GetVisibilityTester(*scene);

  EXPECT_EQ(&spectrum, light.Emission(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                                iris::Vector(0.0, 0.0, 1.0)),
                                      *visibility_tester,
                                      iris::testing::GetSpectralAllocator()));
}

TEST(ListSceneTest, AreaLightEmissionMisses) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  iris::SceneObjects::Builder builder;
  builder.Add(iris::MakeReferenceCounted<TestLightedGeometry>(
      iris::Point(0.0, 0.0, 0.0), 1.0, &emissive_material));
  auto scene_objects = builder.Build();

  EXPECT_EQ(1u, scene_objects.NumLights());
  const auto& light = scene_objects.GetLight(0);

  auto scene = iris::scenes::ListScene::Builder::Create()->Build(scene_objects);
  auto visibility_tester = iris::testing::GetVisibilityTester(*scene);

  EXPECT_EQ(nullptr, light.Emission(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                              iris::Vector(0.0, 0.0, 1.0)),
                                    *visibility_tester,
                                    iris::testing::GetSpectralAllocator()));
}

TEST(ListSceneTest, AreaLightSampleRngFails) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  iris::SceneObjects::Builder builder;
  builder.Add(iris::MakeReferenceCounted<TestLightedGeometry>(
      iris::Point(0.0, 0.0, 0.0), 1.0, &emissive_material, false));
  auto scene_objects = builder.Build();

  EXPECT_EQ(1u, scene_objects.NumLights());
  const auto& light = scene_objects.GetLight(0);

  auto scene = iris::scenes::ListScene::Builder::Create()->Build(scene_objects);
  auto visibility_tester = iris::testing::GetVisibilityTester(*scene);

  iris::random::MockRandom random;
  EXPECT_FALSE(light.Sample(iris::Point(0.0, 0.0, 0.0), random,
                            *visibility_tester,
                            iris::testing::GetSpectralAllocator()));
}

TEST(ListSceneTest, AreaLightSampleNotVisible) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  iris::SceneObjects::Builder builder;
  builder.Add(iris::MakeReferenceCounted<TestLightedGeometry>(
      iris::Point(0.0, 0.0, 1.0), 1.0, &emissive_material));
  auto scene_objects = builder.Build();

  EXPECT_EQ(1u, scene_objects.NumLights());
  const auto& light = scene_objects.GetLight(0);

  auto scene = iris::scenes::ListScene::Builder::Create()->Build(scene_objects);
  auto visibility_tester = iris::testing::GetVisibilityTester(*scene);

  iris::random::MockRandom random;
  EXPECT_FALSE(light.Sample(iris::Point(0.0, 0.0, 0.0), random,
                            *visibility_tester,
                            iris::testing::GetSpectralAllocator()));
}

TEST(ListSceneTest, AreaLightSampleWorld) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  EXPECT_CALL(emissive_material, Evaluate(testing::_, testing::_))
      .WillOnce(testing::Return(&spectrum));

  iris::SceneObjects::Builder builder;
  builder.Add(iris::MakeReferenceCounted<TestLightedGeometry>(
      iris::Point(0.0, 0.0, 1.0), 1.0, &emissive_material));
  auto scene_objects = builder.Build();

  EXPECT_EQ(1u, scene_objects.NumLights());
  const auto& light = scene_objects.GetLight(0);

  auto scene = iris::scenes::ListScene::Builder::Create()->Build(scene_objects);
  auto visibility_tester = iris::testing::GetVisibilityTester(*scene);

  iris::random::MockRandom random;
  auto result =
      light.Sample(iris::Point(0.0, 0.0, 0.0), random, *visibility_tester,
                   iris::testing::GetSpectralAllocator());
  EXPECT_TRUE(result);
  EXPECT_EQ(&spectrum, &result->emission);
  EXPECT_EQ(1.0, result->pdf);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result->to_light);
}

TEST(ListSceneTest, AreaLightSampleWithTransform) {
  iris::spectra::MockSpectrum spectrum;
  iris::emissive_materials::MockEmissiveMaterial emissive_material;

  EXPECT_CALL(emissive_material, Evaluate(testing::_, testing::_))
      .WillOnce(testing::Return(&spectrum));

  iris::SceneObjects::Builder builder;
  builder.Add(iris::MakeReferenceCounted<TestLightedGeometry>(
                  iris::Point(0.0, 0.0, -1.0), 1.0, &emissive_material),
              iris::Matrix::Scalar(1.0, 1.0, -1.0).value());
  auto scene_objects = builder.Build();

  EXPECT_EQ(1u, scene_objects.NumLights());
  const auto& light = scene_objects.GetLight(0);

  auto scene = iris::scenes::ListScene::Builder::Create()->Build(scene_objects);
  auto visibility_tester = iris::testing::GetVisibilityTester(*scene);

  iris::random::MockRandom random;
  auto result =
      light.Sample(iris::Point(0.0, 0.0, 0.0), random, *visibility_tester,
                   iris::testing::GetSpectralAllocator());
  EXPECT_TRUE(result);
  EXPECT_EQ(&spectrum, &result->emission);
  EXPECT_EQ(1.0, result->pdf);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result->to_light);
}