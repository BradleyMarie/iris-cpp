#include "iris/ray.h"

#include "googletest/include/gtest/gtest.h"

TEST(RayTest, Create) {
  auto origin = iris::Point(1.0, 2.0, 3.0);
  auto direction = iris::Vector(4.0, 5.0, 6.0);
  auto ray = iris::Ray(origin, direction);
  EXPECT_EQ(origin, ray.origin);
  EXPECT_EQ(direction, ray.direction);
}

TEST(RayTest, Endpoint) {
  auto origin = iris::Point(1.0, 2.0, 3.0);
  auto direction = iris::Vector(4.0, 5.0, 6.0);
  auto ray = iris::Ray(origin, direction);
  EXPECT_EQ(iris::Point(9.0, 12.0, 15.0), ray.Endpoint(2.0));
}

TEST(RayTest, Normalize) {
  auto origin = iris::Point(1.0, 2.0, 3.0);
  EXPECT_EQ(iris::Ray(origin, iris::Vector(1.0, 0.0, 0.0)),
            iris::Normalize(iris::Ray(origin, iris::Vector(2.0, 0.0, 0.0))));
  EXPECT_EQ(iris::Ray(origin, iris::Vector(0.0, 1.0, 0.0)),
            iris::Normalize(iris::Ray(origin, iris::Vector(0.0, 1.0, 0.0))));
  EXPECT_EQ(iris::Ray(origin, iris::Vector(0.0, 0.0, 1.0)),
            iris::Normalize(iris::Ray(origin, iris::Vector(0.0, 0.0, 1.0))));
}