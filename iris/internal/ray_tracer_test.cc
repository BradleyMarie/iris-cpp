#include "iris/internal/ray_tracer.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/scenes/list_scene.h"

class TestGeometry : public iris::Geometry {
 public:
  TestGeometry(const iris::Ray& ray) : expected_ray_(ray) {}

 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const override {
    EXPECT_EQ(expected_ray_, ray);
    return &hit_allocator.Allocate(nullptr, 1.0, 2, 3);
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
};

TEST(RayTracerTest, NoGeometry) {
  iris::internal::RayTracer ray_tracer;
  auto scene = iris::scenes::ListScene::Builder::Create()->Build();
  EXPECT_EQ(nullptr, ray_tracer.Trace(iris::Ray(iris::Point(0.0, 0.0, 0.0),
                                                iris::Vector(1.0, 1.0, 1.0)),
                                      0.0, 1.0, *scene));
}

TEST(RayTracerTest, WithGeometry) {
  iris::Ray ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));
  auto geometry = std::make_unique<TestGeometry>(ray);
  auto* geometry_ptr = geometry.get();

  auto builder = iris::scenes::ListScene::Builder::Create();
  builder->Add(std::move(geometry));
  auto scene = builder->Build();

  iris::internal::RayTracer ray_tracer;
  auto* hit = ray_tracer.Trace(ray, 0.0, 2.0, *scene);
  ASSERT_NE(nullptr, hit);
  EXPECT_EQ(1.0, hit->distance);
  EXPECT_EQ(nullptr, hit->next);
  EXPECT_EQ(2u, hit->front);
  EXPECT_EQ(3u, hit->back);
  EXPECT_EQ(geometry_ptr, hit->geometry);
  EXPECT_EQ(nullptr, hit->model_to_world);
  EXPECT_EQ(nullptr, hit->additional_data);
}