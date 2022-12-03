#include "frontends/pbrt/integrators/path.h"

#include "frontends/pbrt/build_objects.h"
#include "frontends/pbrt/spectrum_managers/test_spectrum_manager.h"
#include "googletest/include/gtest/gtest.h"

TEST(Path, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::integrators::g_path_builder, tokenizer,
      spectrum_manager, texture_manager);
  EXPECT_NE(nullptr, result.integrator.get());
  EXPECT_NE(nullptr, result.light_scene_builder.get());
  EXPECT_FALSE(result.pixel_bounds);
}

TEST(Path, TooLowMaxDepth) {
  std::stringstream input("\"integer maxdepth\" -1");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::integrators::g_path_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: maxdepth");
}

TEST(Path, TooHighMaxDepth) {
  std::stringstream input("\"integer maxdepth\" 256");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::integrators::g_path_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: maxdepth");
}

TEST(Path, NegativeRrthreshold) {
  std::stringstream input("\"float rrthreshold\" -1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::integrators::g_path_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: rrthreshold");
}

TEST(Path, TooBigRrthreshold) {
  std::stringstream input("\"float rrthreshold\" 2.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::integrators::g_path_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: rrthreshold");
}

TEST(Path, BadLightSampleStrategy) {
  std::stringstream input("\"string lightsamplestrategy\" \"aaa\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::integrators::g_path_builder, tokenizer,
          spectrum_manager, texture_manager),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Unsupported value for parameter lightsamplestrategy: aaa");
}

TEST(Path, BadPixelBounds) {
  std::stringstream input("\"integer pixelbounds\" [-1 2 3 4]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::integrators::g_path_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Negative value in parameter list: pixelbounds");
}

TEST(Path, AllSpecified) {
  std::stringstream input(
      "\"integer maxdepth\" 4 \"float rrthreshold\" 0.5 \"string "
      "lightsamplestrategy\" \"uniform\" \"integer pixelbounds\" [1 2 3 4]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::integrators::g_path_builder, tokenizer,
      spectrum_manager, texture_manager);
  EXPECT_NE(nullptr, result.integrator.get());
  EXPECT_NE(nullptr, result.light_scene_builder.get());
  EXPECT_EQ(1u, result.pixel_bounds.value().at(0));
  EXPECT_EQ(2u, result.pixel_bounds.value().at(1));
  EXPECT_EQ(3u, result.pixel_bounds.value().at(2));
  EXPECT_EQ(4u, result.pixel_bounds.value().at(3));
}