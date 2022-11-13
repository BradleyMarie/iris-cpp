#include "frontends/pbrt/texture_manager.h"

#include "googletest/include/gtest/gtest.h"

TEST(TextureManager, FloatTextureGetFails) {
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(texture_manager.GetFloatTexture("test"),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: No float texture defined with name: test");
}

TEST(TextureManager, ReflectorTextureGetFails) {
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(texture_manager.GetFloatTexture("test"),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: No float texture defined with name: test");
}

TEST(TextureManager, FloatTextureGetSucceeds) {
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::ReferenceCounted<iris::textures::ValueTexture2D<iris::visual>> texture;
  texture_manager.Put("test", texture);
  EXPECT_EQ(nullptr, texture_manager.GetFloatTexture("test").Get());
}

TEST(TextureManager, ReflectorTextureGetSucceeds) {
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::ReferenceCounted<iris::textures::PointerTexture2D<
      iris::Reflector, iris::SpectralAllocator>>
      texture;
  texture_manager.Put("test", texture);
  EXPECT_EQ(nullptr, texture_manager.GetReflectorTexture("test").Get());
}