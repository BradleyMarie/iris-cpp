#include "iris/internal/ray_tracer.h"

#include "googletest/include/gtest/gtest.h"

class TestGeometry : public iris::Geometry {
 public:
  TestGeometry(const iris::Ray& ray) : expected_ray_(ray) {}

 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const {
    EXPECT_EQ(expected_ray_, ray);
    return &hit_allocator.Allocate(nullptr, 1.0, 2, 3);
  }

  virtual iris::Vector ComputeSurfaceNormal(const iris::Point& hit_point,
                                            iris::face_t face,
                                            const void* additional_data) const {
    EXPECT_FALSE(true);
    return iris::Vector(1.0, 0.0, 0.0);
  }

  virtual std::span<const iris::face_t> GetFaces() const {
    static const iris::face_t faces[] = {1};
    EXPECT_FALSE(true);
    return faces;
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

  virtual std::span<const iris::face_t> GetFaces() const {
    static const iris::face_t faces[] = {1};
    EXPECT_FALSE(true);
    return faces;
  }

 private:
  iris::Geometry* geometry_;
};

TEST(RayTracerTest, NoGeometry) {
  iris::internal::RayTracer ray_tracer;
  TestScene scene(nullptr);
  EXPECT_EQ(nullptr, ray_tracer.Trace(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                                iris::Vector(1.0, 1.0, 1.0)),
                                      0.0, 1.0, scene));
}

TEST(RayTracerTest, WithGeometry) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  TestGeometry geometry(ray);

  iris::internal::RayTracer ray_tracer;
  TestScene scene(&geometry);

  auto* hit = ray_tracer.Trace(ray, 0.0, 2.0, scene);
  ASSERT_NE(nullptr, hit);
  EXPECT_EQ(1.0, hit->distance);
  EXPECT_EQ(nullptr, hit->next);
  EXPECT_EQ(2u, hit->front);
  EXPECT_EQ(3u, hit->back);
  EXPECT_EQ(&geometry, hit->geometry);
  EXPECT_EQ(nullptr, hit->model_to_world);
  EXPECT_EQ(nullptr, hit->additional_data);
}