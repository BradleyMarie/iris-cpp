#include "iris/scene_objects.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/environmental_lights/mock_environmental_light.h"
#include "iris/lights/mock_light.h"

class TestGeometry : public iris::Geometry {
 public:
  TestGeometry(bool emissive) : emissive_(emissive) {}

  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const override {
    EXPECT_FALSE(true);
    return nullptr;
  }

  iris::Vector ComputeSurfaceNormal(
      const iris::Point& hit_point, iris::face_t face,
      const void* additional_data) const override {
    EXPECT_FALSE(true);
    return iris::Vector(1.0, 0.0, 0.0);
  }

  bool IsEmissive(iris::face_t face) const override { return emissive_; }

  std::optional<iris::visual_t> ComputeArea(iris::face_t face) const override {
    return 1.0;
  }

  std::span<const iris::face_t> GetFaces() const override {
    static const iris::face_t faces[] = {1};
    return faces;
  }

 private:
  bool emissive_;
};

TEST(SceneObjects, Build) {
  auto geom0 = iris::MakeReferenceCounted<TestGeometry>(false);
  auto geom1 = iris::MakeReferenceCounted<TestGeometry>(false);
  auto geom2 = iris::MakeReferenceCounted<TestGeometry>(false);
  auto geom3 = iris::MakeReferenceCounted<TestGeometry>(true);
  auto light0 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  auto light1 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  auto environmental_light = iris::MakeReferenceCounted<
      iris::environmental_lights::MockEnvironmentalLight>();
  auto matrix0 = iris::Matrix::Identity();
  auto matrix1 = iris::Matrix::Translation(1.0, 2.0, 3.0).value();
  auto matrix2 = iris::Matrix::Scalar(1.0, 2.0, 3.0).value();

  iris::SceneObjects::Builder builder;

  builder.Add(iris::ReferenceCounted<iris::lights::MockLight>());
  builder.Add(iris::ReferenceCounted<iris::Geometry>(), matrix0);

  builder.Add(light0);
  builder.Add(light1);

  builder.Add(geom0, matrix1);
  builder.Add(geom1, matrix2);
  builder.Add(geom2, matrix2);
  builder.Add(geom3, matrix0);

  builder.Set(environmental_light);

  auto scene_objects = builder.Build();
  ASSERT_EQ(4u, scene_objects.NumGeometry());
  EXPECT_EQ(geom0.Get(), &scene_objects.GetGeometry(0).first);
  EXPECT_EQ(matrix1, *scene_objects.GetGeometry(0).second);
  EXPECT_EQ(geom1.Get(), &scene_objects.GetGeometry(1).first);
  EXPECT_EQ(matrix2, *scene_objects.GetGeometry(1).second);
  EXPECT_EQ(geom2.Get(), &scene_objects.GetGeometry(2).first);
  EXPECT_EQ(matrix2, *scene_objects.GetGeometry(2).second);
  EXPECT_EQ(geom3.Get(), &scene_objects.GetGeometry(3).first);
  EXPECT_EQ(nullptr, scene_objects.GetGeometry(3).second);
  EXPECT_EQ(*scene_objects.GetGeometry(1).second,
            *scene_objects.GetGeometry(2).second);
  ASSERT_EQ(4u, scene_objects.NumLights());
  EXPECT_EQ(light0.Get(), &scene_objects.GetLight(0));
  EXPECT_EQ(light1.Get(), &scene_objects.GetLight(1));
  EXPECT_EQ(environmental_light.Get(), scene_objects.GetEnvironmentalLight());

  // Reuse builder
  geom0 = iris::MakeReferenceCounted<TestGeometry>(false);
  geom1 = iris::MakeReferenceCounted<TestGeometry>(false);
  geom2 = iris::MakeReferenceCounted<TestGeometry>(false);
  geom3 = iris::MakeReferenceCounted<TestGeometry>(true);
  light0 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  light1 = iris::MakeReferenceCounted<iris::lights::MockLight>();

  builder.Add(light0);
  builder.Add(light1);

  builder.Add(geom0, matrix1);
  builder.Add(geom1, matrix2);
  builder.Add(geom2, matrix2);
  builder.Add(geom3, matrix0);

  scene_objects = builder.Build();
  ASSERT_EQ(4u, scene_objects.NumGeometry());
  EXPECT_EQ(geom0.Get(), &scene_objects.GetGeometry(0).first);
  EXPECT_EQ(matrix1, *scene_objects.GetGeometry(0).second);
  EXPECT_EQ(geom1.Get(), &scene_objects.GetGeometry(1).first);
  EXPECT_EQ(matrix2, *scene_objects.GetGeometry(1).second);
  EXPECT_EQ(geom2.Get(), &scene_objects.GetGeometry(2).first);
  EXPECT_EQ(matrix2, *scene_objects.GetGeometry(2).second);
  EXPECT_EQ(geom3.Get(), &scene_objects.GetGeometry(3).first);
  EXPECT_EQ(nullptr, scene_objects.GetGeometry(3).second);
  EXPECT_EQ(*scene_objects.GetGeometry(1).second,
            *scene_objects.GetGeometry(2).second);
  ASSERT_EQ(3u, scene_objects.NumLights());
  EXPECT_EQ(light0.Get(), &scene_objects.GetLight(0));
  EXPECT_EQ(light1.Get(), &scene_objects.GetLight(1));
  EXPECT_EQ(nullptr, scene_objects.GetEnvironmentalLight());
}