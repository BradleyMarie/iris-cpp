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

TEST(ListSceneTest, SceneTestSuite) {
  auto builder = iris::scenes::ListScene::Builder::Create();
  iris::testing::SceneTestSuite(*builder);
}