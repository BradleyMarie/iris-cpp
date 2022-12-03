#include "frontends/pbrt/samplers/random.h"

#include "frontends/pbrt/build_objects.h"
#include "frontends/pbrt/spectrum_managers/test_spectrum_manager.h"
#include "googletest/include/gtest/gtest.h"

TEST(Random, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::samplers::g_random_builder, tokenizer,
      spectrum_manager, texture_manager);
  ASSERT_TRUE(result);
  EXPECT_EQ(16u, result->SamplesPerPixel());
}

TEST(Random, TooLowPixelSamples) {
  std::stringstream input("\"integer pixelsamples\" -1");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::samplers::g_random_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: pixelsamples");
}

TEST(Random, TooHighPixelSamples) {
  std::stringstream input("\"integer pixelsamples\" 4294967297");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::samplers::g_random_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: pixelsamples");
}

TEST(Random, AllSpecified) {
  std::stringstream input("\"integer pixelsamples\" 4");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::samplers::g_random_builder, tokenizer,
      spectrum_manager, texture_manager);
  ASSERT_TRUE(result);
  EXPECT_EQ(4u, result->SamplesPerPixel());
}