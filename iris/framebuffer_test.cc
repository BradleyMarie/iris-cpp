#include "iris/framebuffer.h"

#include "googletest/include/gtest/gtest.h"

TEST(FramebufferTest, Create) {
  iris::Framebuffer framebuffer(2, 3);
  auto size = framebuffer.Size();
  EXPECT_EQ(2u, size.first);
  EXPECT_EQ(3u, size.second);
}

TEST(FramebufferTest, Get) {
  iris::Framebuffer framebuffer(2, 2);
  auto expected_color = iris::Color(0.0, 0.0, 0.0, iris::Color::CIE_XYZ);
  EXPECT_EQ(expected_color, framebuffer.Get(0, 0));
  EXPECT_EQ(expected_color, framebuffer.Get(0, 1));
  EXPECT_EQ(expected_color, framebuffer.Get(1, 0));
  EXPECT_EQ(expected_color, framebuffer.Get(1, 1));
}

TEST(FramebufferTest, Set) {
  iris::Framebuffer framebuffer(1, 1);
  EXPECT_EQ(iris::Color(0.0, 0.0, 0.0, iris::Color::CIE_XYZ),
            framebuffer.Get(0, 0));
  framebuffer.Set(0, 0, iris::Color(1.0, 2.0, 3.0, iris::Color::LINEAR_SRGB));
  EXPECT_EQ(iris::Color(1.0, 2.0, 3.0, iris::Color::LINEAR_SRGB),
            framebuffer.Get(0, 0));
}