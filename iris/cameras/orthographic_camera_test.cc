#include "iris/cameras/orthographic_camera.h"

#include "googletest/include/gtest/gtest.h"

TEST(OrthographicCameraTest, HasLens) {
  iris::cameras::OrthographicCamera camera(
      iris::Matrix::Identity(),
      std::array<iris::geometric_t, 4>({-1.0, -1.0, 1.0, 1.0}));
  EXPECT_FALSE(camera.HasLens());
}

TEST(OrthographicCameraTest, NoTransform) {
  iris::cameras::OrthographicCamera camera(
      iris::Matrix::Identity(),
      std::array<iris::geometric_t, 4>({-1.0, -1.0, 1.0, 1.0}));

  auto top_left = camera.Compute({0.0, 0.0}, nullptr);
  EXPECT_EQ(-1.0, top_left.origin.x);
  EXPECT_EQ(1.0, top_left.origin.y);
  EXPECT_EQ(0.0, top_left.origin.z);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), top_left.direction);

  auto top_right = camera.Compute({1.0, 0.0}, nullptr);
  EXPECT_EQ(1.0, top_right.origin.x);
  EXPECT_EQ(1.0, top_right.origin.y);
  EXPECT_EQ(0.0, top_right.origin.z);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), top_right.direction);

  auto bottom_left = camera.Compute({0.0, 1.0}, nullptr);
  EXPECT_EQ(-1.0, bottom_left.origin.x);
  EXPECT_EQ(-1.0, bottom_left.origin.y);
  EXPECT_EQ(0.0, bottom_left.origin.z);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), bottom_left.direction);

  auto bottom_right = camera.Compute({1.0, 1.0}, nullptr);
  EXPECT_EQ(1.0, bottom_right.origin.x);
  EXPECT_EQ(-1.0, bottom_right.origin.y);
  EXPECT_EQ(0.0, bottom_right.origin.z);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), bottom_right.direction);
}

TEST(OrthographicCameraTest, Transformed) {
  iris::cameras::OrthographicCamera camera(
      iris::Matrix::Translation(1.0, 2.0, 3.0).value(),
      std::array<iris::geometric_t, 4>({-0.5, -0.5, 0.5, 0.5}));

  auto top_left = camera.Compute({0.0, 0.0}, nullptr);
  EXPECT_EQ(0.5, top_left.origin.x);
  EXPECT_EQ(2.5, top_left.origin.y);
  EXPECT_EQ(3.0, top_left.origin.z);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), top_left.direction);

  auto top_right = camera.Compute({1.0, 0.0}, nullptr);
  EXPECT_EQ(1.5, top_right.origin.x);
  EXPECT_EQ(2.5, top_right.origin.y);
  EXPECT_EQ(3.0, top_right.origin.z);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), top_right.direction);

  auto bottom_left = camera.Compute({0.0, 1.0}, nullptr);
  EXPECT_EQ(0.5, bottom_left.origin.x);
  EXPECT_EQ(1.5, bottom_left.origin.y);
  EXPECT_EQ(3.0, bottom_left.origin.z);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), bottom_left.direction);

  auto bottom_right = camera.Compute({1.0, 1.0}, nullptr);
  EXPECT_EQ(1.5, bottom_right.origin.x);
  EXPECT_EQ(1.5, bottom_right.origin.y);
  EXPECT_EQ(3.0, bottom_right.origin.z);
  EXPECT_EQ(iris::Vector(0.0, 0.0, 1.0), bottom_right.direction);
}