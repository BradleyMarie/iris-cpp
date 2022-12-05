#include "iris/cameras/perspective_camera.h"

#include "googletest/include/gtest/gtest.h"

TEST(PerspectiveCameraTest, HasLens) {
  iris::cameras::PerspectiveCamera camera(
      iris::Matrix::Identity(),
      std::array<iris::geometric_t, 4>({-1.0, -1.0, 1.0, 1.0}), 90.0);
  EXPECT_FALSE(camera.HasLens());
}

TEST(PerspectiveCameraTest, FourCorners) {
  iris::cameras::PerspectiveCamera camera(
      iris::Matrix::Identity(),
      std::array<iris::geometric_t, 4>({-1.0, -1.0, 1.0, 1.0}), 90.0);

  auto top_left = camera.Compute({0.0, 0.0}, nullptr);
  EXPECT_EQ(iris::Point(-1.0, 1.0, 0.0), top_left.origin);
  EXPECT_NEAR(-0.57735025882720947, top_left.direction.x, 0.001);
  EXPECT_NEAR(0.57735025882720947, top_left.direction.y, 0.001);
  EXPECT_NEAR(0.57735025882720947, top_left.direction.z, 0.001);

  auto top_right = camera.Compute({1.0, 0.0}, nullptr);
  EXPECT_EQ(iris::Point(1.0, 1.0, 0.0), top_right.origin);
  EXPECT_NEAR(0.57735025882720947, top_right.direction.x, 0.001);
  EXPECT_NEAR(0.57735025882720947, top_right.direction.y, 0.001);
  EXPECT_NEAR(0.57735025882720947, top_right.direction.z, 0.001);

  auto bottom_left = camera.Compute({0.0, 1.0}, nullptr);
  EXPECT_EQ(iris::Point(-1.0, -1.0, 0.0), bottom_left.origin);
  EXPECT_NEAR(-0.57735025882720947, bottom_left.direction.x, 0.001);
  EXPECT_NEAR(-0.57735025882720947, bottom_left.direction.y, 0.001);
  EXPECT_NEAR(0.57735025882720947, bottom_left.direction.z, 0.001);

  auto bottom_right = camera.Compute({1.0, 1.0}, nullptr);
  EXPECT_EQ(iris::Point(1.0, -1.0, 0.0), bottom_right.origin);
  EXPECT_NEAR(0.57735025882720947, bottom_right.direction.x, 0.001);
  EXPECT_NEAR(-0.57735025882720947, bottom_right.direction.y, 0.001);
  EXPECT_NEAR(0.57735025882720947, bottom_right.direction.z, 0.001);
}

TEST(PerspectiveCameraTest, Center) {
  iris::cameras::PerspectiveCamera camera(
      iris::Matrix::Identity(),
      std::array<iris::geometric_t, 4>({-1.0, -1.0, 1.0, 1.0}), 90.0);
  auto ray = camera.Compute({0.5, 0.5}, nullptr);
  EXPECT_EQ(iris::Point(0.0, 0.0, 0.0), ray.origin);
  EXPECT_NEAR(0.0, ray.direction.x, 0.001);
  EXPECT_NEAR(0.0, ray.direction.y, 0.001);
  EXPECT_NEAR(1.0, ray.direction.z, 0.001);
}

TEST(PerspectiveCameraTest, ScaledCenter) {
  iris::cameras::PerspectiveCamera camera(
      iris::Matrix::Scalar(2.0, 1.0, 2.0).value(),
      std::array<iris::geometric_t, 4>({-1.0, -1.0, 1.0, 1.0}), 90.0);
  auto ray = camera.Compute({0.5, 0.5}, nullptr);
  EXPECT_EQ(iris::Point(0.0, 0.0, 0.0), ray.origin);
  EXPECT_NEAR(0.0, ray.direction.x, 0.001);
  EXPECT_NEAR(0.0, ray.direction.y, 0.001);
  EXPECT_NEAR(1.0, ray.direction.z, 0.001);
}

TEST(PerspectiveCameraTest, TranslatedCenter) {
  iris::cameras::PerspectiveCamera camera(
      iris::Matrix::Translation(1.0, 2.0, 3.0).value(),
      std::array<iris::geometric_t, 4>({-1.0, -1.0, 1.0, 1.0}), 90.0);
  auto ray = camera.Compute({0.5, 0.5}, nullptr);
  EXPECT_EQ(iris::Point(1.0, 2.0, 3.0), ray.origin);
  EXPECT_NEAR(0.0, ray.direction.x, 0.001);
  EXPECT_NEAR(0.0, ray.direction.y, 0.001);
  EXPECT_NEAR(1.0, ray.direction.z, 0.001);
}