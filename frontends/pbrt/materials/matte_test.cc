#include "frontends/pbrt/materials/matte.h"

#include "frontends/pbrt/build_objects.h"
#include "frontends/pbrt/spectrum_managers/test_spectrum_manager.h"
#include "googletest/include/gtest/gtest.h"

static iris::pbrt_frontend::TextureManager g_texture_manager;

TEST(Matte, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result0 = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::materials::g_matte_builder, tokenizer,
      spectrum_manager, texture_manager, texture_manager);
  EXPECT_TRUE(result0);

  auto result1 = iris::pbrt_frontend::BuildObject(
      *result0, tokenizer, spectrum_manager, texture_manager, texture_manager);
  EXPECT_TRUE(result1);

  auto result2 = iris::pbrt_frontend::BuildObject(
      *result0, tokenizer, spectrum_manager, texture_manager, texture_manager);
  EXPECT_EQ(result1, result2);
}

TEST(Matte, WithDefaults) {
  std::stringstream input("\"float Kd\" 1.0 \"float sigma\" 0.5");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result0 = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::materials::g_matte_builder, tokenizer,
      spectrum_manager, texture_manager, texture_manager);
  EXPECT_TRUE(result0);

  auto result1 = iris::pbrt_frontend::BuildObject(
      *result0, tokenizer, spectrum_manager, texture_manager, texture_manager);
  EXPECT_TRUE(result1);

  auto result2 = iris::pbrt_frontend::BuildObject(
      *result0, tokenizer, spectrum_manager, texture_manager, texture_manager);
  EXPECT_EQ(result1, result2);
}

TEST(Matte, OverridesDefaults) {
  std::stringstream input0("");
  iris::pbrt_frontend::Tokenizer tokenizer0(input0);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result0 = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::materials::g_matte_builder, tokenizer0,
      spectrum_manager, texture_manager, texture_manager);
  EXPECT_TRUE(result0);

  auto result1 = iris::pbrt_frontend::BuildObject(
      *result0, tokenizer0, spectrum_manager, texture_manager, texture_manager);
  EXPECT_TRUE(result1);

  std::stringstream input1("\"float Kd\" 1.0 \"float sigma\" 0.5");
  iris::pbrt_frontend::Tokenizer tokenizer1(input1);

  auto result2 = iris::pbrt_frontend::BuildObject(
      *result0, tokenizer1, spectrum_manager, texture_manager, texture_manager);
  EXPECT_NE(result1, result2);
}