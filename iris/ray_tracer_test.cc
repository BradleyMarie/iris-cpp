#include "iris/ray_tracer.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/ray_tracer.h"

const iris::Bsdf* g_bsdf = reinterpret_cast<iris::Bsdf*>(1);
const iris::Spectrum* g_spectrum = reinterpret_cast<iris::Spectrum*>(2);

class TestEmissiveMaterial : public iris::EmissiveMaterial {
 public:
  TestEmissiveMaterial(std::array<iris::geometric_t, 2> expected)
      : expected_(expected) {}

  const iris::Spectrum* Compute(
      const iris::TextureCoordinates& texture_coordinates) const {
    EXPECT_EQ(expected_, texture_coordinates.uv);
    EXPECT_FALSE(texture_coordinates.derivatives);
    return g_spectrum;
  }

 private:
  std::array<iris::geometric_t, 2> expected_;
};

class TestMaterial : public iris::Material {
 public:
  TestMaterial(std::array<iris::geometric_t, 2> expected)
      : expected_(expected) {}

  const iris::Bsdf* Compute(
      const iris::TextureCoordinates& texture_coordinates) const {
    EXPECT_EQ(expected_, texture_coordinates.uv);
    EXPECT_FALSE(texture_coordinates.derivatives);
    return g_bsdf;
  }

 private:
  std::array<iris::geometric_t, 2> expected_;
};

class TestNormalMap : public iris::NormalMap {
 public:
  TestNormalMap(std::array<iris::geometric_t, 2> expected)
      : expected_(expected) {}

  iris::Vector Compute(const iris::TextureCoordinates& texture_coordinates,
                       const iris::Vector& surface_normal) const {
    EXPECT_EQ(expected_, texture_coordinates.uv);
    EXPECT_FALSE(texture_coordinates.derivatives);
    EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), surface_normal);
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
                   iris::HitAllocator& hit_allocator) const {
    EXPECT_EQ(expected_ray_, ray);
    return &hit_allocator.Allocate(nullptr, 1.0, 2u, 3u);
  }

  virtual iris::Vector ComputeSurfaceNormal(const iris::Point& hit_point,
                                            iris::face_t face,
                                            const void* additional_data) const {
    EXPECT_EQ(expected_hit_point_, hit_point);
    EXPECT_EQ(nullptr, additional_data);
    return iris::Vector(2.0, 0.0, 0.0);
  }

  std::optional<iris::TextureCoordinates> ComputeTextureCoordinates(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const {
    EXPECT_EQ(expected_hit_point_, hit_point);
    EXPECT_EQ(2u, face);
    EXPECT_EQ(nullptr, additional_data);
    return texture_coordinates_;
  }

  virtual std::variant<iris::Vector, const iris::NormalMap*>
  ComputeShadingNormal(iris::face_t face, const void* additional_data) const {
    EXPECT_EQ(2u, face);
    EXPECT_EQ(nullptr, additional_data);
    return normal_map_;
  }

  virtual const iris::Material* GetMaterial(iris::face_t face,
                                            const void* additional_data) const {
    EXPECT_EQ(2u, face);
    return material_;
  }

  virtual const iris::EmissiveMaterial* GetEmissiveMaterial(
      iris::face_t face, const void* additional_data) const {
    EXPECT_EQ(2u, face);
    return emissive_material_;
  }

  virtual std::span<const iris::face_t> GetFaces() const {
    static const iris::face_t faces[] = {1};
    EXPECT_FALSE(true);
    return faces;
  }

  iris::Ray expected_ray_;
  iris::Point expected_hit_point_;
  const iris::Material* material_;
  const iris::EmissiveMaterial* emissive_material_;
  const std::variant<iris::Vector, const iris::NormalMap*> normal_map_;
  const std::optional<iris::TextureCoordinates> texture_coordinates_;
};

class TestScene : public iris::Scene {
 public:
  TestScene(iris::Geometry* geometry) : geometry_(geometry) {}

  void Trace(const iris::Ray& ray, iris::Intersector& intersector) const {
    if (geometry_) {
      intersector.Intersect(*geometry_);
    }
  }

 private:
  iris::Geometry* geometry_;
};

class TestSceneWithTransform : public iris::Scene {
 public:
  TestSceneWithTransform(iris::Geometry* geometry)
      : geometry_(geometry),
        matrix_(iris::Matrix::Scalar(-1.0, 1.0, 1.0).value()) {}

  void Trace(const iris::Ray& ray, iris::Intersector& intersector) const {
    if (geometry_) {
      intersector.Intersect(*geometry_, matrix_);
    }
  }

 private:
  iris::Geometry* geometry_;
  iris::Matrix matrix_;
};

TEST(RayTracerTest, NoGeometry) {
  iris::internal::RayTracer internal_ray_tracer;
  TestScene scene(nullptr);
  iris::RayTracer ray_tracer(scene, 0.0, internal_ray_tracer);

  EXPECT_FALSE(ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0))));
}

TEST(RayTracerTest, Minimal) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestGeometry geometry(ray, iris::Point(1.0, 1.0, 1.0));

  iris::internal::RayTracer internal_ray_tracer;
  TestScene scene(&geometry);
  iris::RayTracer ray_tracer(scene, 0.0, internal_ray_tracer);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  ASSERT_TRUE(result);
  EXPECT_EQ(nullptr, result->bsdf);
  EXPECT_EQ(nullptr, result->emission);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result->hit_point);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->surface_normal);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->shading_normal);
}

TEST(RayTracerTest, WithTransform) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(-1.0, 1.0, 1.0));
  TestGeometry geometry(ray, iris::Point(-1.0, 1.0, 1.0));

  iris::internal::RayTracer internal_ray_tracer;
  TestSceneWithTransform scene(&geometry);
  iris::RayTracer ray_tracer(scene, 0.0, internal_ray_tracer);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  ASSERT_TRUE(result);
  EXPECT_EQ(nullptr, result->bsdf);
  EXPECT_EQ(nullptr, result->emission);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result->hit_point);
  EXPECT_EQ(iris::Vector(-1.0, 0.0, 0.0), result->surface_normal);
  EXPECT_EQ(iris::Vector(-1.0, 0.0, 0.0), result->shading_normal);
}

TEST(RayTracerTest, WithTextureCoordinates) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestMaterial material({1.0, 1.0});
  TestGeometry geometry(ray, iris::Point(1.0, 1.0, 1.0), &material, nullptr,
                        nullptr, iris::TextureCoordinates{{1.0, 1.0}});

  iris::internal::RayTracer internal_ray_tracer;
  TestScene scene(&geometry);
  iris::RayTracer ray_tracer(scene, 0.0, internal_ray_tracer);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  ASSERT_TRUE(result);
  EXPECT_EQ(g_bsdf, result->bsdf);
  EXPECT_EQ(nullptr, result->emission);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result->hit_point);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->surface_normal);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->shading_normal);
}

TEST(RayTracerTest, WithMaterial) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestMaterial material({0.0, 0.0});
  TestGeometry geometry(ray, iris::Point(1.0, 1.0, 1.0), &material);

  iris::internal::RayTracer internal_ray_tracer;
  TestScene scene(&geometry);
  iris::RayTracer ray_tracer(scene, 0.0, internal_ray_tracer);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  ASSERT_TRUE(result);
  EXPECT_EQ(g_bsdf, result->bsdf);
  EXPECT_EQ(nullptr, result->emission);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result->hit_point);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->surface_normal);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->shading_normal);
}

TEST(RayTracerTest, WithEmissiveMaterial) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestEmissiveMaterial emissive_material({0.0, 0.0});
  TestGeometry geometry(ray, iris::Point(1.0, 1.0, 1.0), nullptr,
                        &emissive_material);

  iris::internal::RayTracer internal_ray_tracer;
  TestScene scene(&geometry);
  iris::RayTracer ray_tracer(scene, 0.0, internal_ray_tracer);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  ASSERT_TRUE(result);
  EXPECT_EQ(nullptr, result->bsdf);
  EXPECT_EQ(g_spectrum, result->emission);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result->hit_point);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->surface_normal);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->shading_normal);
}

TEST(RayTracerTest, WithNormal) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestGeometry geometry(ray, iris::Point(1.0, 1.0, 1.0), nullptr, nullptr,
                        iris::Vector(0.0, 1.0, 0.0));

  iris::internal::RayTracer internal_ray_tracer;
  TestScene scene(&geometry);
  iris::RayTracer ray_tracer(scene, 0.0, internal_ray_tracer);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  ASSERT_TRUE(result);
  EXPECT_EQ(nullptr, result->bsdf);
  EXPECT_EQ(nullptr, result->emission);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result->hit_point);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->surface_normal);
  EXPECT_EQ(iris::Vector(0.0, 1.0, 0.0), result->shading_normal);
}

TEST(RayTracerTest, WithNormalMap) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestNormalMap normal_map({0.0, 0.0});
  TestGeometry geometry(ray, iris::Point(1.0, 1.0, 1.0), nullptr, nullptr,
                        &normal_map);

  iris::internal::RayTracer internal_ray_tracer;
  TestScene scene(&geometry);
  iris::RayTracer ray_tracer(scene, 0.0, internal_ray_tracer);

  auto result = ray_tracer.Trace(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)));
  ASSERT_TRUE(result);
  EXPECT_EQ(nullptr, result->bsdf);
  EXPECT_EQ(nullptr, result->emission);
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0), result->hit_point);
  EXPECT_EQ(iris::Vector(2.0, 0.0, 0.0), result->surface_normal);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), result->shading_normal);
}