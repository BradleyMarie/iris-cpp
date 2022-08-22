#include "iris/geometry.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/hit_arena.h"

class TestGeometry final : public iris::Geometry {
 public:
  TestGeometry(const iris::Ray& ray, const iris::Geometry* nested)
      : expected_ray_(ray), nested_(nested) {}

 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const {
    EXPECT_EQ(expected_ray_, ray);
    if (nested_) {
      auto* nested_hit = nested_->Trace(hit_allocator);
      return &hit_allocator.Allocate(nested_hit, 1.0, 2, 3);
    }
    return &hit_allocator.Allocate(nullptr, 2.0, 3, 4);
  }

  virtual iris::Vector ComputeSurfaceNormal(const iris::Point& hit_point,
                                            iris::face_t face,
                                            const void* additional_data) const {
    EXPECT_FALSE(true);
    return iris::Vector(1.0, 0.0, 0.0);
  }

  iris::Ray expected_ray_;
  const iris::Geometry* nested_;
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

TEST(GeometryTrace, ComputeTextureCoordinates) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  EXPECT_FALSE(
      geom.ComputeTextureCoordinates(iris::Point(0.0, 0.0, 0.0), 0, nullptr)
          .has_value());
}

TEST(GeometryTrace, ComputeShadingNormal) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  auto normal = geom.ComputeShadingNormal(0, nullptr);
  ASSERT_TRUE(std::holds_alternative<iris::NormalMap*>(normal));
  EXPECT_EQ(nullptr, std::get<iris::NormalMap*>(normal));
}

TEST(GeometryTrace, GetMaterial) {
  TestGeometry geom(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)),
      nullptr);
  EXPECT_EQ(nullptr, geom.GetMaterial(0, nullptr));
}