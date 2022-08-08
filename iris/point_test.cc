#include "iris/point.h"

#include "googletest/include/gtest/gtest.h"

TEST(PointTest, Create) {
  auto point = iris::Point(1.0, 2.0, 3.0);
  EXPECT_EQ(1.0, point.x);
  EXPECT_EQ(2.0, point.y);
  EXPECT_EQ(3.0, point.z);
}

TEST(PointTest, Subscript) {
  auto point = iris::Point(1.0, 2.0, 3.0);
  EXPECT_EQ(1.0, point[0]);
  EXPECT_EQ(2.0, point[1]);
  EXPECT_EQ(3.0, point[2]);
}

TEST(PointTest, SubtractPoint) {
  auto minuend = iris::Point(3.0, 4.0, 5.0);
  auto subtrahend = iris::Point(2.0, 1.0, 3.0);
  EXPECT_EQ(iris::Vector(1.0, 3.0, 2.0), minuend - subtrahend);
}

TEST(PointTest, SubtractVector) {
  auto minuend = iris::Point(3.0, 4.0, 5.0);
  auto subtrahend = iris::Vector(2.0, 1.0, 3.0);
  EXPECT_EQ(iris::Point(1.0, 3.0, 2.0), minuend - subtrahend);
}

TEST(PointTest, Add) {
  auto addend0 = iris::Point(3.0, 4.0, 5.0);
  auto addend1 = iris::Vector(2.0, 2.0, 3.0);
  EXPECT_EQ(iris::Point(5.0, 6.0, 8.0), addend0 + addend1);
}