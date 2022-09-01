#include "iris/scenes/list_scene.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/testing/scene.h"

class TestGeometry final : public iris::Geometry {
 public:
  TestGeometry(const iris::Ray& ray, bool* intersected)
      : expected_ray_(ray), intersected_(intersected) {
    *intersected = false;
  }

 private:
  iris::Hit* Trace(const iris::Ray& ray,
                   iris::HitAllocator& hit_allocator) const override {
    EXPECT_EQ(expected_ray_, ray);
    *intersected_ = true;
    return nullptr;
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
  bool* intersected_;
};

TEST(ListSceneTest, Iterators) {
  auto builder = iris::scenes::ListScene::Builder::Create();

  auto matrix0 = iris::Matrix::Translation(1.0, 0.0, 0.0).value();
  auto matrix1 = iris::Matrix::Identity();

  iris::Ray unused_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(1.0, 1.0, 1.0));

  bool unused_bool;
  auto geom0 = std::make_unique<TestGeometry>(unused_ray, &unused_bool);
  auto geom1 = std::make_unique<TestGeometry>(unused_ray, &unused_bool);

  auto geom0_ptr = geom0.get();
  auto geom1_ptr = geom1.get();

  builder->Add(std::move(geom0), matrix0);
  builder->Add(std::move(geom1), matrix1);
  auto result = builder->Build();

  size_t index = 0u;
  for (const auto& [geometry, matrix] : *result) {
    size_t current = index++;
    if (current == 0u) {
      EXPECT_EQ(geom0_ptr, &geometry);
      EXPECT_EQ(matrix0, *matrix);
    } else if (current == 1u) {
      EXPECT_EQ(geom1_ptr, &geometry);
      EXPECT_EQ(nullptr, matrix);
    } else {
      EXPECT_TRUE(false);
    }
  }
}

TEST(ListSceneTest, SceneTestSuite) {
  auto builder = iris::scenes::ListScene::Builder::Create();
  iris::testing::SceneTestSuite(*builder);
}