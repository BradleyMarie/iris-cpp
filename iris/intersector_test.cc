#include "iris/intersector.h"

#include <limits>
#include <memory>

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/hit_arena.h"

static const iris::Ray ray(iris::Point(0.0, 0.0, 0.0),
                           iris::Vector(1.0, 1.0, 1.0));

class TestGeometry final : public iris::Geometry {
 public:
  TestGeometry(iris::geometric_t distance) : distance_(distance) {}

 private:
  iris::Hit* Trace(const iris::Ray& trace_ray,
                   iris::HitAllocator& hit_allocator) const {
    EXPECT_EQ(ray, trace_ray);
    return &hit_allocator.Allocate(nullptr, distance_, 2, 3);
  }

  virtual iris::Vector ComputeSurfaceNormal(const iris::Point& hit_point,
                                            iris::face_t face,
                                            const void* additional_data) const {
    EXPECT_FALSE(true);
    return iris::Vector(1.0, 0.0, 0.0);
  }

  iris::geometric_t distance_;
};

TEST(InspectorTest, Initialize) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(ray, 1.0, 2.0, arena, &closest_hit);

  EXPECT_EQ(nullptr, closest_hit);
  EXPECT_EQ(1.0, intersector.MinimumDistance());
  EXPECT_EQ(2.0, intersector.MaximumDistance());
}

TEST(IntersectorTest, TooClose) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(ray, 1.0, 2.0, arena, &closest_hit);

  TestGeometry geometry(0.0);
  intersector.Intersect(geometry);

  EXPECT_EQ(nullptr, closest_hit);
  EXPECT_EQ(1.0, intersector.MinimumDistance());
  EXPECT_EQ(2.0, intersector.MaximumDistance());
};

TEST(IntersectorTest, TooFar) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(ray, 1.0, 2.0, arena, &closest_hit);

  TestGeometry geometry(3.0);
  intersector.Intersect(geometry);

  EXPECT_EQ(nullptr, closest_hit);
  EXPECT_EQ(1.0, intersector.MinimumDistance());
  EXPECT_EQ(2.0, intersector.MaximumDistance());
};

TEST(IntersectorTest, Hits) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(ray, 0.0, 2.0, arena, &closest_hit);

  TestGeometry geometry(1.0);
  intersector.Intersect(geometry);

  EXPECT_EQ(0.0, intersector.MinimumDistance());
  EXPECT_EQ(1.0, intersector.MaximumDistance());

  auto* full_hit = static_cast<iris::internal::Hit*>(closest_hit);
  EXPECT_EQ(1.0, closest_hit->distance);
  EXPECT_EQ(2u, full_hit->front);
  EXPECT_EQ(3u, full_hit->back);
  EXPECT_EQ(&geometry, full_hit->geometry);
  EXPECT_EQ(nullptr, full_hit->model_to_world);
};

TEST(IntersectorTest, HitsIgnoreFarther) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(ray, 0.0, 2.0, arena, &closest_hit);

  TestGeometry geometry0(1.0);
  intersector.Intersect(geometry0);

  TestGeometry geometry1(2.0);
  intersector.Intersect(geometry1);

  EXPECT_EQ(0.0, intersector.MinimumDistance());
  EXPECT_EQ(1.0, intersector.MaximumDistance());

  auto* full_hit = static_cast<iris::internal::Hit*>(closest_hit);
  EXPECT_EQ(1.0, closest_hit->distance);
  EXPECT_EQ(2u, full_hit->front);
  EXPECT_EQ(3u, full_hit->back);
  EXPECT_EQ(&geometry0, full_hit->geometry);
  EXPECT_EQ(nullptr, full_hit->model_to_world);
};

TEST(IntersectorTest, KeepsCloser) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(ray, 0.0, 3.0, arena, &closest_hit);

  TestGeometry geometry0(2.0);
  intersector.Intersect(geometry0);

  EXPECT_EQ(0.0, intersector.MinimumDistance());
  EXPECT_EQ(2.0, intersector.MaximumDistance());

  TestGeometry geometry1(1.0);
  intersector.Intersect(geometry1);

  EXPECT_EQ(0.0, intersector.MinimumDistance());
  EXPECT_EQ(1.0, intersector.MaximumDistance());

  auto* full_hit = static_cast<iris::internal::Hit*>(closest_hit);
  EXPECT_EQ(1.0, closest_hit->distance);
  EXPECT_EQ(2u, full_hit->front);
  EXPECT_EQ(3u, full_hit->back);
  EXPECT_EQ(&geometry1, full_hit->geometry);
  EXPECT_EQ(nullptr, full_hit->model_to_world);
};

static const iris::Matrix model_to_world =
    iris::Matrix::Translation(1.0, 1.0, 1.0).value();
static const iris::Ray transformed_ray(iris::Point(1.0, 1.0, 1.0),
                                       iris::Vector(1.0, 1.0, 1.0));

TEST(IntersectorTest, TransformedTooClose) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(transformed_ray, 1.0, 2.0, arena, &closest_hit);

  TestGeometry geometry(0.0);
  intersector.Intersect(geometry, model_to_world);

  EXPECT_EQ(nullptr, closest_hit);
  EXPECT_EQ(1.0, intersector.MinimumDistance());
  EXPECT_EQ(2.0, intersector.MaximumDistance());
};

TEST(IntersectorTest, TransformedTooFar) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(transformed_ray, 1.0, 2.0, arena, &closest_hit);

  TestGeometry geometry(3.0);
  intersector.Intersect(geometry, model_to_world);

  EXPECT_EQ(nullptr, closest_hit);
  EXPECT_EQ(1.0, intersector.MinimumDistance());
  EXPECT_EQ(2.0, intersector.MaximumDistance());
};

TEST(IntersectorTest, TransformedHits) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(transformed_ray, 0.0, 2.0, arena, &closest_hit);

  TestGeometry geometry(1.0);
  intersector.Intersect(geometry, model_to_world);

  EXPECT_EQ(0.0, intersector.MinimumDistance());
  EXPECT_EQ(1.0, intersector.MaximumDistance());

  auto* full_hit = static_cast<iris::internal::Hit*>(closest_hit);
  EXPECT_EQ(1.0, closest_hit->distance);
  EXPECT_EQ(2u, full_hit->front);
  EXPECT_EQ(3u, full_hit->back);
  EXPECT_EQ(&geometry, full_hit->geometry);
  EXPECT_EQ(&model_to_world, full_hit->model_to_world);
};

TEST(IntersectorTest, TransformedHitsIgnoreFarther) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(transformed_ray, 0.0, 2.0, arena, &closest_hit);

  TestGeometry geometry0(1.0);
  intersector.Intersect(geometry0, model_to_world);

  TestGeometry geometry1(2.0);
  intersector.Intersect(geometry1, model_to_world);

  EXPECT_EQ(0.0, intersector.MinimumDistance());
  EXPECT_EQ(1.0, intersector.MaximumDistance());

  auto* full_hit = static_cast<iris::internal::Hit*>(closest_hit);
  EXPECT_EQ(1.0, closest_hit->distance);
  EXPECT_EQ(2u, full_hit->front);
  EXPECT_EQ(3u, full_hit->back);
  EXPECT_EQ(&geometry0, full_hit->geometry);
  EXPECT_EQ(&model_to_world, full_hit->model_to_world);
};

TEST(IntersectorTest, TransformedKeepsCloser) {
  auto arena = iris::internal::HitArena();

  iris::Hit* closest_hit;
  iris::Intersector intersector(transformed_ray, 0.0, 3.0, arena, &closest_hit);

  TestGeometry geometry0(2.0);
  intersector.Intersect(geometry0, model_to_world);

  EXPECT_EQ(0.0, intersector.MinimumDistance());
  EXPECT_EQ(2.0, intersector.MaximumDistance());

  TestGeometry geometry1(1.0);
  intersector.Intersect(geometry1, model_to_world);

  EXPECT_EQ(0.0, intersector.MinimumDistance());
  EXPECT_EQ(1.0, intersector.MaximumDistance());

  auto* full_hit = static_cast<iris::internal::Hit*>(closest_hit);
  EXPECT_EQ(1.0, closest_hit->distance);
  EXPECT_EQ(2u, full_hit->front);
  EXPECT_EQ(3u, full_hit->back);
  EXPECT_EQ(&geometry1, full_hit->geometry);
  EXPECT_EQ(&model_to_world, full_hit->model_to_world);
};