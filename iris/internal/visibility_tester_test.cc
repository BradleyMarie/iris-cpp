#include "iris/internal/visibility_tester.h"

#include <optional>

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/ray_tracer.h"
#include "iris/scenes/list_scene.h"

static bool g_override_face = false;
static iris::face_t g_front_face = 1;
static const uint32_t g_data = 0xDEADBEEF;
static bool g_second_hit = false;

class TestEmissiveMaterial final : public iris::EmissiveMaterial {
 public:
  TestEmissiveMaterial(std::array<iris::geometric, 2> expected,
                       const iris::Spectrum* spectrum = nullptr)
      : expected_(expected), spectrum_(spectrum) {}

  const iris::Spectrum* Compute(
      const iris::TextureCoordinates& texture_coordinates) const override {
    EXPECT_EQ(expected_, texture_coordinates.uv);
    EXPECT_FALSE(texture_coordinates.derivatives);
    return spectrum_;
  }

 private:
  std::array<iris::geometric, 2> expected_;
  const iris::Spectrum* spectrum_;
};

class TestGeometry final : public iris::Geometry {
 public:
  TestGeometry(std::optional<iris::Ray> ray = std::nullopt,
               const iris::Point& hit_point = iris::Point(1.0, 0.0, 0.0),
               const iris::EmissiveMaterial* emissive_material = nullptr,
               const std::optional<iris::TextureCoordinates>&
                   texture_coordinates = std::nullopt)
      : expected_ray_(ray),
        expected_hit_point_(hit_point),
        emissive_material_(emissive_material),
        texture_coordinates_(texture_coordinates) {}

 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const override {
    if (expected_ray_) {
      EXPECT_EQ(*expected_ray_, ray);
    }

    if (g_override_face) {
      return &hit_allocator.Allocate(nullptr, 1.0, g_front_face++, 2, g_data);
    }

    auto* good_hit = &hit_allocator.Allocate(nullptr, 1.0, 1, 2, g_data);
    if (!g_second_hit) {
      return good_hit;
    }

    return &hit_allocator.Allocate(good_hit, 0.5, 100, 200, g_data);
  }

  iris::Vector ComputeSurfaceNormal(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const override {
    EXPECT_EQ(expected_hit_point_, hit_point);
    EXPECT_EQ(1u, face);
    EXPECT_EQ(g_data, *static_cast<const uint32_t*>(additional_data));
    return iris::Vector(-1.0, 0.0, 0.0);
  }

  std::optional<iris::TextureCoordinates> ComputeTextureCoordinates(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const override {
    EXPECT_EQ(expected_hit_point_, hit_point);
    EXPECT_EQ(1u, face);
    EXPECT_EQ(g_data, *static_cast<const uint32_t*>(additional_data));
    return texture_coordinates_;
  }

  const iris::EmissiveMaterial* GetEmissiveMaterial(
      iris::face_t face, const void* additional_data) const override {
    EXPECT_EQ(1u, face);
    EXPECT_EQ(g_data, *static_cast<const uint32_t*>(additional_data));
    return emissive_material_;
  }

  std::span<const iris::face_t> GetFaces() const override {
    static const iris::face_t faces[] = {1, 2};
    EXPECT_FALSE(true);
    return faces;
  }

  std::optional<iris::Ray> expected_ray_;
  iris::Point expected_hit_point_;
  const iris::EmissiveMaterial* emissive_material_;
  const std::optional<iris::TextureCoordinates> texture_coordinates_;
};

TEST(VisibilityTesterTest, MissesGeometry) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  auto geometry = std::make_unique<TestGeometry>(model_ray);

  auto scene = iris::scenes::ListScene::Builder::Create()->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 50.0, ray_tracer);

  EXPECT_FALSE(visibility_tester.Visible(world_ray, *geometry, nullptr, 1, 1.0,
                                         nullptr));
}

TEST(VisibilityTesterTest, WrongFace) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  auto geometry = std::make_unique<TestGeometry>(model_ray);

  auto scene = iris::scenes::ListScene::Builder::Create()->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  EXPECT_FALSE(visibility_tester.Visible(world_ray, *geometry, nullptr, 2, 1.0,
                                         nullptr));
}

TEST(VisibilityTesterTest, SucceedsSecondHit) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  const iris::Spectrum* spectrum = reinterpret_cast<iris::Spectrum*>(2);
  TestEmissiveMaterial emissive_material({0.0, 0.0}, spectrum);

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>(
      model_ray, iris::Point(1.0, 0.0, 0.0), &emissive_material);
  auto geometry_ptr = geometry.get();
  builder->Add(std::move(geometry));

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  g_second_hit = true;
  EXPECT_FALSE(visibility_tester.Visible(world_ray, *geometry_ptr, nullptr, 1,
                                         1.0, nullptr));
  g_second_hit = false;
}

TEST(VisibilityTesterTest, SceneTraceMisses) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  auto geometry = std::make_unique<TestGeometry>(model_ray);

  auto scene = iris::scenes::ListScene::Builder::Create()->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  EXPECT_FALSE(visibility_tester.Visible(world_ray, *geometry, nullptr, 1, 1.0,
                                         nullptr));
}

TEST(VisibilityTesterTest, SceneTraceWrongGeometry) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>(model_ray);
  auto geometry_ptr = geometry.get();
  builder->Add(std::make_unique<TestGeometry>(model_ray));

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  EXPECT_FALSE(visibility_tester.Visible(world_ray, *geometry_ptr, nullptr, 1,
                                         1.0, nullptr));
}

TEST(VisibilityTesterTest, SceneTraceWrongMatrix) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>();
  auto geometry_ptr = geometry.get();
  builder->Add(std::move(geometry),
               iris::Matrix::Translation(1.0, 0.0, 0.0).value());

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  EXPECT_FALSE(visibility_tester.Visible(world_ray, *geometry_ptr, nullptr, 1,
                                         1.0, nullptr));
}

TEST(VisibilityTesterTest, SceneTraceWrongFace) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>(model_ray);
  auto geometry_ptr = geometry.get();
  builder->Add(std::move(geometry));

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  g_override_face = true;
  EXPECT_FALSE(visibility_tester.Visible(world_ray, *geometry_ptr, nullptr,
                                         g_front_face, 1.0, nullptr));
  g_override_face = false;
}

TEST(VisibilityTesterTest, NoEmissiveMaterial) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>(model_ray);
  auto geometry_ptr = geometry.get();
  builder->Add(std::move(geometry));

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  EXPECT_FALSE(visibility_tester.Visible(world_ray, *geometry_ptr, nullptr, 1,
                                         1.0, nullptr));
}

TEST(VisibilityTesterTest, NoSpectrum) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  TestEmissiveMaterial emissive_material({0.0, 0.0});

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>(
      model_ray, iris::Point(1.0, 0.0, 0.0), &emissive_material);
  auto geometry_ptr = geometry.get();
  builder->Add(std::move(geometry));

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  EXPECT_FALSE(visibility_tester.Visible(world_ray, *geometry_ptr, nullptr, 1,
                                         1.0, nullptr));
}

TEST(VisibilityTesterTest, Succeeds) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  const iris::Spectrum* spectrum = reinterpret_cast<iris::Spectrum*>(2);
  TestEmissiveMaterial emissive_material({0.0, 0.0}, spectrum);

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>(
      model_ray, iris::Point(1.0, 0.0, 0.0), &emissive_material);
  auto geometry_ptr = geometry.get();
  builder->Add(std::move(geometry));

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  auto result = visibility_tester.Visible(world_ray, *geometry_ptr, nullptr, 1,
                                          1.0, nullptr);
  ASSERT_TRUE(result);
  EXPECT_EQ(spectrum, &result->emission);
  EXPECT_EQ(iris::Point(1.0, 0.0, 0.0), result->hit_point);
}

TEST(VisibilityTesterTest, SucceedsWithPdf) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  const iris::Spectrum* spectrum = reinterpret_cast<iris::Spectrum*>(2);
  TestEmissiveMaterial emissive_material({0.0, 0.0}, spectrum);

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>(
      model_ray, iris::Point(1.0, 0.0, 0.0), &emissive_material);
  auto geometry_ptr = geometry.get();
  builder->Add(std::move(geometry));

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  iris::visual pdf;
  auto result = visibility_tester.Visible(world_ray, *geometry_ptr, nullptr, 1,
                                          0.5, &pdf);
  ASSERT_TRUE(result);
  EXPECT_EQ(spectrum, &result->emission);
  EXPECT_EQ(iris::Point(1.0, 0.0, 0.0), result->hit_point);
  EXPECT_EQ(2.0, pdf);
}

TEST(VisibilityTesterTest, SucceedsWithTransformWithPdf) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.5, 0.0, 0.0));

  const iris::Spectrum* spectrum = reinterpret_cast<iris::Spectrum*>(2);
  TestEmissiveMaterial emissive_material({0.0, 0.0}, spectrum);

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>(
      model_ray, iris::Point(0.5, 0.0, 0.0), &emissive_material);
  auto geometry_ptr = geometry.get();
  builder->Add(std::move(geometry),
               iris::Matrix::Scalar(2.0, 1.0, 1.0).value());

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  iris::visual pdf;
  auto result = visibility_tester.Visible(
      world_ray, *geometry_ptr, (*scene->begin()).second, 1, 0.5, &pdf);
  ASSERT_TRUE(result);
  EXPECT_EQ(spectrum, &result->emission);
  EXPECT_EQ(iris::Point(1.0, 0.0, 0.0), result->hit_point);
  EXPECT_EQ(2.0, pdf);
}

TEST(VisibilityTesterTest, SucceedsWithCoordinates) {
  iris::Ray world_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));
  iris::Ray model_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 0.0, 0.0));

  const iris::Spectrum* spectrum = reinterpret_cast<iris::Spectrum*>(2);
  TestEmissiveMaterial emissive_material({0.5, 0.5}, spectrum);

  auto builder = iris::scenes::ListScene::Builder::Create();

  auto geometry = std::make_unique<TestGeometry>(
      model_ray, iris::Point(1.0, 0.0, 0.0), &emissive_material,
      iris::TextureCoordinates{{0.5, 0.5}});
  auto geometry_ptr = geometry.get();
  builder->Add(std::move(geometry));

  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  iris::internal::VisibilityTester visibility_tester(*scene, 0.0, ray_tracer);

  auto result = visibility_tester.Visible(world_ray, *geometry_ptr, nullptr, 1,
                                          1.0, nullptr);
  ASSERT_TRUE(result);
  EXPECT_EQ(spectrum, &result->emission);
  EXPECT_EQ(iris::Point(1.0, 0.0, 0.0), result->hit_point);
}