#include "iris/textures/constant_texture.h"

#include <memory>
#include <vector>

#include "googletest/include/gtest/gtest.h"

TEST(ConstantValueTexture2DTest, Test) {
  iris::textures::ConstantValueTexture2D<float> texture(1.0);
  EXPECT_EQ(1.0, texture.Evaluate(iris::TextureCoordinates{}));
}

TEST(PointerValueTexture2DTest, Test) {
  auto value = std::make_unique<int>(1);
  iris::textures::ConstantPointerTexture2D<std::unique_ptr<int>, int> texture(
      std::move(value), *value.get());
  EXPECT_EQ(1, *texture.Evaluate(iris::TextureCoordinates{}));
}

TEST(PointerValueTexture2DTest, TestWithArgs) {
  auto value = std::make_unique<int>(1);
  iris::textures::ConstantPointerTexture2D<std::unique_ptr<int>, int,
                                           std::vector<int>>
      texture(std::move(value), *value.get());

  std::vector<int> empty_vector;
  EXPECT_EQ(1, *texture.Evaluate(iris::TextureCoordinates{}, empty_vector));
}