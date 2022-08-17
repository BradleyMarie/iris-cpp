#include "iris/visibility_tester.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/ray_tracer.h"

class TestGeometry : public iris::Geometry {
 public:
  TestGeometry(const iris::Ray& ray) : expected_ray_(ray) {}

 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const {
    EXPECT_EQ(expected_ray_, ray);
    return &hit_allocator.Allocate(nullptr, 1.0, 2, 3);
  }

  iris::Ray expected_ray_;
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

TEST(RayTracerTest, NoGeometry) {
  iris::internal::RayTracer ray_tracer;
  TestScene scene(nullptr);
  iris::VisibilityTester visibility_tester(scene, 0.0, ray_tracer);

  EXPECT_TRUE(visibility_tester.Visible(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0))));
  EXPECT_TRUE(visibility_tester.Visible(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)), 1.0));
}

TEST(RayTracerTest, WithGeometry) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestGeometry geometry(ray);

  iris::internal::RayTracer ray_tracer;
  TestScene scene(&geometry);
  iris::VisibilityTester visibility_tester(scene, 0.0, ray_tracer);

  EXPECT_TRUE(visibility_tester.Visible(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)), 0.5));
  EXPECT_FALSE(visibility_tester.Visible(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0)), 1.5));
  EXPECT_FALSE(visibility_tester.Visible(
      iris::Ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0))));
}