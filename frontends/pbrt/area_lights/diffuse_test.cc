#include "frontends/pbrt/area_lights/diffuse.h"

#include "frontends/pbrt/build_objects.h"
#include "frontends/pbrt/spectrum_managers/test_spectrum_manager.h"
#include "googletest/include/gtest/gtest.h"

static iris::pbrt_frontend::TextureManager g_texture_manager;

TEST(Diffuse, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result0 = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::area_lights::g_diffuse_builder, tokenizer,
      spectrum_manager, texture_manager,
      static_cast<iris::pbrt_frontend::SpectrumManager&>(spectrum_manager));
  EXPECT_TRUE(result0.first);
  EXPECT_FALSE(result0.second);
}

TEST(Diffuse, TooLowSamples) {
  std::stringstream input("\"integer samples\" 0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::area_lights::g_diffuse_builder, tokenizer,
          spectrum_manager, texture_manager,
          static_cast<iris::pbrt_frontend::SpectrumManager&>(spectrum_manager)),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Out of range value for parameter: samples");
}

TEST(Diffuse, TooHighSamples) {
  std::stringstream input("\"integer samples\" 256");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::area_lights::g_diffuse_builder, tokenizer,
          spectrum_manager, texture_manager,
          static_cast<iris::pbrt_frontend::SpectrumManager&>(spectrum_manager)),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Out of range value for parameter: samples");
}

TEST(Diffuse, SetsAll) {
  std::stringstream input(
      "\"color L\" [2.0 2.0 2.0] \"integer samples\" 2 "
      "\"bool twosided\" \"true\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result0 = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::area_lights::g_diffuse_builder, tokenizer,
      spectrum_manager, texture_manager,
      static_cast<iris::pbrt_frontend::SpectrumManager&>(spectrum_manager));
  EXPECT_TRUE(result0.first);
  EXPECT_TRUE(result0.second);
}