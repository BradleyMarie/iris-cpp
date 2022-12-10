#include "iris/reflectors/uniform_reflector.h"

#include "googletest/include/gtest/gtest.h"

TEST(UniformReflector, Reflectance) {
  iris::reflectors::UniformReflector reflector(0.25);
  EXPECT_EQ(0.25, reflector.Albedo());
  EXPECT_EQ(0.25, reflector.Reflectance(0.5));
  EXPECT_EQ(0.25, reflector.Reflectance(1.0));
  EXPECT_EQ(0.25, reflector.Reflectance(1.5));
}