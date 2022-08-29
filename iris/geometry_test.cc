#include "iris/geometry.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/hit_arena.h"

class TestGeometry final : public iris::Geometry {
 public:
  TestGeometry(const iris::Ray& ray, const iris::Geometry* nested)
      : expected_ray_(ray), nested_(nested) {}

 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const override {
    EXPECT_EQ(expected_ray_, ray);
    if (nested_) {
      auto* nested_hit = nested_->Trace(hit_allocator);
      return &hit_allocator.Allocate(nested_hit, 1.0, 2, 3);
    }
    return &hit_allocator.Allocate(nullptr, 2.0, 3, 4);
  }

  iris::Vector ComputeSurfaceNormal(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const override {
    EXPECT_FALSE(true);
    return iris::Vector(1.0, 0.0, 0.0);
  }

  std::span<const iris::face_t> GetFaces() const override {
    static const iris::face_t faces[] = {1};
    EXPECT_FALSE(true);
    return faces;
  }

  iris::Ray expected_ray_;
  const iris::Geometry* nested_;
};

class TestRandom final : public iris::Random {
  result_type min() const override {
    EXPECT_FALSE(true);
    return 0u;
  }

  result_type max() const override {
    EXPECT_FALSE(true);
    return 0u;
  }

  result_type operator()() override {
    EXPECT_FALSE(true);
    return 0u;
  }

  void discard(unsigned long long z) override { EXPECT_FALSE(true); }

  std::unique_ptr<Random> Replicate() override {
    EXPECT_FALSE(true);
    return nullptr;
  }
};

TEST(GeometryTest, Trace) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  auto arena = iris::internal::HitArena();
  iris::HitAllocator allocator(ray, arena);

  TestGeometry nested_geom(ray, nullptr);
  TestGeometry geom(ray, &nested_geom);

  iris::Geometry* base_geom = &geom;
  auto* hit = base_geom->Trace(allocator);

  auto* full_hit = static_cast<iris::internal::Hit*>(hit);
  EXPECT_EQ(1.0, hit->distance);
  EXPECT_EQ(2u, full_hit->front);
  EXPECT_EQ(3u, full_hit->back);
  EXPECT_EQ(base_geom, full_hit->geometry);

  ASSERT_NE(nullptr, hit->next);
  auto* next_hit = hit->next;
  auto* next_full_hit = static_cast<iris::internal::Hit*>(next_hit);
  EXPECT_EQ(2.0, next_hit->distance);
  EXPECT_EQ(3u, next_full_hit->front);
  EXPECT_EQ(4u, next_full_hit->back);
  EXPECT_EQ(&nested_geom, next_full_hit->geometry);
};

TEST(GeometryTest, ComputeTextureCoordinates) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  EXPECT_FALSE(
      geom.ComputeTextureCoordinates(iris::Point(0.0, 0.0, 0.0), 0, nullptr)
          .has_value());
}

TEST(GeometryTest, ComputeShadingNormal) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  auto normal = geom.ComputeShadingNormal(0, nullptr);
  ASSERT_TRUE(std::holds_alternative<const iris::NormalMap*>(normal));
  EXPECT_EQ(nullptr, std::get<const iris::NormalMap*>(normal));
}

TEST(GeometryTest, GetMaterial) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  EXPECT_EQ(nullptr, geom.GetMaterial(0, nullptr));
}

TEST(GeometryTest, IsEmissive) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  EXPECT_FALSE(geom.IsEmissive(0));
}

TEST(GeometryTest, GetEmissiveMaterial) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  EXPECT_EQ(nullptr, geom.GetEmissiveMaterial(0, nullptr));
}

TEST(GeometryTest, SampleFace) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  TestRandom rng;
  EXPECT_FALSE(geom.SampleFace(0, rng));
}

TEST(GeometryTest, ComputeArea) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  TestRandom rng;
  EXPECT_FALSE(geom.ComputeArea(0));
}