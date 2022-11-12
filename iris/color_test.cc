#include "iris/color.h"

#include "googletest/include/gtest/gtest.h"

TEST(ColorTest, Create) {
  auto color = iris::Color(1.0, 2.0, 3.0, iris::Color::CIE_XYZ);
  EXPECT_EQ(1.0, color.x);
  EXPECT_EQ(2.0, color.y);
  EXPECT_EQ(3.0, color.z);
  EXPECT_EQ(iris::Color::CIE_XYZ, color.space);
}

TEST(ColorTest, Subscript) {
  auto color = iris::Color(1.0, 2.0, 3.0, iris::Color::CIE_XYZ);
  EXPECT_EQ(1.0, color[0]);
  EXPECT_EQ(2.0, color[1]);
  EXPECT_EQ(3.0, color[2]);
}

TEST(ColorTest, Equality) {
  auto c0 = iris::Color(1.0, 2.0, 3.0, iris::Color::CIE_XYZ);
  auto c1 = iris::Color(2.0, 2.0, 3.0, iris::Color::CIE_XYZ);
  auto c2 = iris::Color(1.0, 3.0, 3.0, iris::Color::CIE_XYZ);
  auto c3 = iris::Color(1.0, 2.0, 4.0, iris::Color::CIE_XYZ);
  auto c4 = iris::Color(1.0, 2.0, 3.0, iris::Color::LINEAR_SRGB);
  EXPECT_EQ(c0, c0);
  EXPECT_NE(c0, c1);
  EXPECT_NE(c0, c2);
  EXPECT_NE(c0, c3);
  EXPECT_NE(c0, c4);
}

TEST(ColorTest, ConvertToSame) {
  auto color = iris::Color(1.0, 2.0, 3.0, iris::Color::CIE_XYZ);
  EXPECT_EQ(color, color.ConvertTo(iris::Color::CIE_XYZ));
}

TEST(ColorTest, ConvertToLinearRgbFromXyz) {
  auto color =
      iris::Color(0.475235015, 0.50000006, 0.544414997, iris::Color::CIE_XYZ);
  auto actual = color.ConvertTo(iris::Color::LINEAR_SRGB);
  EXPECT_EQ(iris::Color::LINEAR_SRGB, actual.space);
  EXPECT_NEAR(0.5, actual.r, 0.001);
  EXPECT_NEAR(0.5, actual.g, 0.001);
  EXPECT_NEAR(0.5, actual.b, 0.001);
}

TEST(ColorTest, ConvertToXyzFromLinearRgb) {
  auto color = iris::Color(0.5, 0.5, 0.5, iris::Color::LINEAR_SRGB);
  auto actual = color.ConvertTo(iris::Color::CIE_XYZ);
  EXPECT_EQ(iris::Color::CIE_XYZ, actual.space);
  EXPECT_NEAR(0.475235015, actual.r, 0.001);
  EXPECT_NEAR(0.50000006, actual.g, 0.001);
  EXPECT_NEAR(0.544414997, actual.b, 0.001);
}