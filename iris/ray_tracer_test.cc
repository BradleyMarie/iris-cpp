#include "iris/ray_tracer.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/bxdfs/mock_bxdf.h"
#include "iris/environmental_lights/mock_environmental_light.h"
#include "iris/internal/arena.h"
#include "iris/internal/ray_tracer.h"
#include "iris/scenes/list_scene.h"
#include "iris/spectra/mock_spectrum.h"

auto g_bxdf = std::make_unique<iris::bxdfs::MockBxdf>();
auto g_spectrum = std::make_unique<iris::spectra::MockSpectrum>();
static const uint32_t g_data = 0xDEADBEEF;

class TestEmissiveMaterial : public iris::EmissiveMaterial {
 public:
  TestEmissiveMaterial(std::array<iris::geometric_t, 2> expected)
      : expected_(expected) {}

  const iris::Spectrum* Evaluate(
      const iris::TextureCoordinates& texture_coordinates,
      iris::SpectralAllocator& spectral_allocator) const override {
    EXPECT_EQ(expected_, texture_coordinates.uv);
    EXPECT_FALSE(texture_coordinates.derivatives);
    return g_spectrum.get();
  }

 private:
  std::array<iris::geometric_t, 2> expected_;
};

class TestMaterial : public iris::Material {
 public:
  TestMaterial(std::array<iris::geometric_t, 2> expected)
      : expected_(expected) {}

  const iris::Bxdf* Evaluate(
      const iris::TextureCoordinates& texture_coordinates,
      iris::SpectralAllocator& spectral_allocator,
      iris::BxdfAllocator& allocator) const override {
    EXPECT_EQ(expected_, texture_coordinates.uv);
    EXPECT_FALSE(texture_coordinates.derivatives);
    return g_bxdf.get();
  }

 private:
  std::array<iris::geometric_t, 2> expected_;
};

class TestNormalMap : public iris::NormalMap {
 public:
  TestNormalMap(std::array<iris::geometric_t, 2> expected)
      : expected_(expected) {}

  iris::Vector Evaluate(const iris::TextureCoordinates& texture_coordinates,
                        const iris::Vector& surface_normal) const override {
    EXPECT_EQ(expected_, texture_coordinates.uv);
    EXPECT_FALSE(texture_coordinates.derivatives);
    EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0), surface_normal);
    return iris::Vector(0.0, 0.0, 1.0);
  }

 private:
  std::array<iris::geometric_t, 2> expected_;
};

class TestGeometry : public iris::Geometry {
 public:
  TestGeometry(
      const iris::Ray& ray, const iris::Point& hit_point,
      const iris::Material* material = nullptr,
      const iris::EmissiveMaterial* emissive_material = nullptr,
      const std::variant<iris::Vector, const iris::NormalMap*>& normal_map =
          nullptr,
      const std::optional<iris::TextureCoordinates>& texture_coordinates =
          std::nullopt)
      : expected_ray_(ray),
        expected_hit_point_(hit_point),
        material_(material),
        emissive_material_(emissive_material),
        normal_map_(normal_map),
        texture_coordinates_(texture_coordinates) {}

 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const override {
    EXPECT_EQ(expected_ray_, ray);
    return &hit_allocator.Allocate(nullptr, 1.0, 2u, 3u, g_data);
  }

  iris::Vector ComputeSurfaceNormal(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const override {
    EXPECT_EQ(expected_hit_point_, hit_point);
    EXPECT_EQ(g_data, *static_cast<const uint32_t*>(additional_data));
    return iris::Vector(1.0, 0.0, 0.0);
  }

  std::optional<iris::TextureCoordinates> ComputeTextureCoordinates(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const override {
    EXPECT_EQ(expected_hit_point_, hit_point);
    EXPECT_EQ(2u, face);
    EXPECT_EQ(g_data, *static_cast<const uint32_t*>(additional_data));
    return texture_coordinates_;
  }

  std::variant<iris::Vector, const iris::NormalMap*> ComputeShadingNormal(
      iris::face_t face, const void* additional_data) const override {
    EXPECT_EQ(2u, face);
    EXPECT_EQ(g_data, *static_cast<const uint32_t*>(additional_data));
    return normal_map_;
  }

  const iris::Material* GetMaterial(
      iris::face_t face, const void* additional_data) const override {
    EXPECT_EQ(2u, face);
    return material_;
  }

  const iris::EmissiveMaterial* GetEmissiveMaterial(
      iris::face_t face, const void* additional_data) const override {
    EXPECT_EQ(2u, face);
    return emissive_material_;
  }

  std::span<const iris::face_t> GetFaces() const override {
    static const iris::face_t faces[] = {1};
    return faces;
  }

  iris::Ray expected_ray_;
  iris::Point expected_hit_point_;
  const iris::Material* material_;
  const iris::EmissiveMaterial* emissive_material_;
  const std::variant<iris::Vector, const iris::NormalMap*> normal_map_;
  const std::optional<iris::TextureCoordinates> texture_coordinates_;
};

TEST(RayTracerTest, NoGeometry) {
  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  auto objects = iris::SceneObjects::Builder().Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);
  iris::RayTracer ray_tracer(*scene, nullptr, 0.0, internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_FALSE(result.emission);
  EXPECT_FALSE(result.surface_intersection);
}

TEST(RayTracerTest, WithEnvironmentalLight) {
  iris::spectra::MockSpectrum spectrum;
  iris::environmental_lights::MockEnvironmentalLight environmental_light;
  EXPECT_CALL(environmental_light,
              Emission(iris::Vector(1.0, 1.0, 1.0), testing::_, testing::_))
      .WillOnce(testing::Return(&spectrum));

  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  auto objects = iris::SceneObjects::Builder().Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);
  iris::RayTracer ray_tracer(*scene, &environmental_light, 0.0,
                             internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_EQ(&spectrum, result.emission);
  EXPECT_FALSE(result.surface_intersection);
}

TEST(RayTracerTest, NoBsdf) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));

  auto builder = iris::SceneObjects::Builder();
  builder.Add(iris::MakeReferenceCounted<TestGeometry>(
      ray, iris::Point(1.0, 1.0, 1.0)));
  auto objects = builder.Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);

  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  iris::RayTracer ray_tracer(*scene, nullptr, 0.0, internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_FALSE(result.emission);
  EXPECT_FALSE(result.surface_intersection);
}

TEST(RayTracerTest, WithEmissiveMaterial) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestEmissiveMaterial emissive_material({0.0, 0.0});

  auto builder = iris::SceneObjects::Builder();
  builder.Add(iris::MakeReferenceCounted<TestGeometry>(
      ray, iris::Point(1.0, 1.0, 1.0), nullptr, &emissive_material));
  auto objects = builder.Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);

  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  iris::RayTracer ray_tracer(*scene, nullptr, 0.0, internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_EQ(g_spectrum.get(), result.emission);
  EXPECT_FALSE(result.surface_intersection);
}

TEST(RayTracerTest, Minimal) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestMaterial material({0.0, 0.0});

  auto builder = iris::SceneObjects::Builder();
  builder.Add(iris::MakeReferenceCounted<TestGeometry>(
      ray, iris::Point(1.0, 1.0, 1.0), &material));
  auto objects = builder.Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);

  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  iris::RayTracer ray_tracer(*scene, nullptr, 0.0, internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_FALSE(result.emission);
  ASSERT_TRUE(result.surface_intersection);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result.surface_intersection->hit_point);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            result.surface_intersection->surface_normal);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            result.surface_intersection->shading_normal);
}

TEST(RayTracerTest, WithTransform) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(-1.0, 1.0, 1.0));
  TestMaterial material({0.0, 0.0});

  auto builder = iris::SceneObjects::Builder();
  builder.Add(iris::MakeReferenceCounted<TestGeometry>(
                  ray, iris::Point(-1.0, 1.0, 1.0), &material),
              iris::Matrix::Scalar(-1.0, 1.0, 1.0).value());
  auto objects = builder.Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);

  TestGeometry geometry(ray, iris::Point(-1.0, 1.0, 1.0));

  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  iris::RayTracer ray_tracer(*scene, nullptr, 0.0, internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_FALSE(result.emission);
  ASSERT_TRUE(result.surface_intersection);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result.surface_intersection->hit_point);
  EXPECT_EQ(iris::Vector(-1.0, 0.0, 0.0),
            result.surface_intersection->surface_normal);
  EXPECT_EQ(iris::Vector(-1.0, 0.0, 0.0),
            result.surface_intersection->shading_normal);
}

TEST(RayTracerTest, WithTextureCoordinates) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestMaterial material({1.0, 1.0});

  auto builder = iris::SceneObjects::Builder();
  builder.Add(iris::MakeReferenceCounted<TestGeometry>(
      ray, iris::Point(1.0, 1.0, 1.0), &material, nullptr, nullptr,
      iris::TextureCoordinates{{1.0, 1.0}}));
  auto objects = builder.Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);

  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  iris::RayTracer ray_tracer(*scene, nullptr, 0.0, internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_FALSE(result.emission);
  ASSERT_TRUE(result.surface_intersection);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result.surface_intersection->hit_point);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            result.surface_intersection->surface_normal);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            result.surface_intersection->shading_normal);
}

TEST(RayTracerTest, WithMaterial) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestMaterial material({0.0, 0.0});

  auto builder = iris::SceneObjects::Builder();
  builder.Add(iris::MakeReferenceCounted<TestGeometry>(
      ray, iris::Point(1.0, 1.0, 1.0), &material));
  auto objects = builder.Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);

  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  iris::RayTracer ray_tracer(*scene, nullptr, 0.0, internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_FALSE(result.emission);
  ASSERT_TRUE(result.surface_intersection);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result.surface_intersection->hit_point);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            result.surface_intersection->surface_normal);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            result.surface_intersection->shading_normal);
}

TEST(RayTracerTest, WithNormal) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestMaterial material({0.0, 0.0});

  auto builder = iris::SceneObjects::Builder();
  builder.Add(iris::MakeReferenceCounted<TestGeometry>(
      ray, iris::Point(1.0, 1.0, 1.0), &material, nullptr,
      iris::Vector(0.0, 1.0, 0.0)));
  auto objects = builder.Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);

  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  iris::RayTracer ray_tracer(*scene, nullptr, 0.0, internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_FALSE(result.emission);
  ASSERT_TRUE(result.surface_intersection);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result.surface_intersection->hit_point);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            result.surface_intersection->surface_normal);
  EXPECT_EQ(iris::Vector(0.0, 1.0, 0.0),
            result.surface_intersection->shading_normal);
}

TEST(RayTracerTest, WithNormalMap) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestMaterial material({0.0, 0.0});
  TestNormalMap normal_map({0.0, 0.0});

  auto builder = iris::SceneObjects::Builder();
  builder.Add(iris::MakeReferenceCounted<TestGeometry>(
      ray, iris::Point(1.0, 1.0, 1.0), &material, nullptr, &normal_map));
  auto objects = builder.Build();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build(objects);

  iris::internal::RayTracer internal_ray_tracer;
  iris::internal::Arena arena;
  iris::RayTracer ray_tracer(*scene, nullptr, 0.0, internal_ray_tracer, arena);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  EXPECT_FALSE(result.emission);
  ASSERT_TRUE(result.surface_intersection);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result.surface_intersection->hit_point);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            result.surface_intersection->surface_normal);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0),
            result.surface_intersection->shading_normal);
}