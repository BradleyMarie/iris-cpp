#include "iris/textures/constant_texture.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/spectra/mock_spectrum.h"

TEST(ConstantValueTexture2DTest, Test) {
  iris::textures::ConstantValueTexture2D<float> texture(1.0);
  EXPECT_EQ(1.0, texture.Evaluate(iris::TextureCoordinates{}));
}

TEST(PointerValueTexture2DTest, Test) {
  auto value = iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();
  iris::textures::ConstantPointerTexture2D<iris::Spectrum> texture(value);
  EXPECT_EQ(value.Get(), texture.Evaluate(iris::TextureCoordinates{}));
}