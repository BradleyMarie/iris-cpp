#include "frontends/pbrt/samplers/stratified.h"

#include "frontends/pbrt/build_objects.h"
#include "frontends/pbrt/spectrum_managers/test_spectrum_manager.h"
#include "googletest/include/gtest/gtest.h"

TEST(Stratified, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::samplers::g_stratified_builder, tokenizer,
      spectrum_manager, texture_manager);
  ASSERT_TRUE(result);
  EXPECT_EQ(4u, result->SamplesPerPixel());
}

TEST(Stratified, TooLowXSamples) {
  std::stringstream input("\"integer xsamples\" -1");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::samplers::g_stratified_builder,
                  tokenizer, spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: xsamples");
}

TEST(Stratified, TooHighXSamples) {
  std::stringstream input("\"integer xsamples\" 65536");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::samplers::g_stratified_builder,
                  tokenizer, spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: xsamples");
}

TEST(Stratified, TooLowYSamples) {
  std::stringstream input("\"integer ysamples\" -1");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::samplers::g_stratified_builder,
                  tokenizer, spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: ysamples");
}

TEST(Stratified, TooHighYSamples) {
  std::stringstream input("\"integer ysamples\" 65536");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::samplers::g_stratified_builder,
                  tokenizer, spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: ysamples");
}

TEST(Stratified, AllSpecified) {
  std::stringstream input(
      "\"integer xsamples\" 65535 \"integer ysamples\" 256 \"bool jitter\" "
      "\"false\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::samplers::g_stratified_builder, tokenizer,
      spectrum_manager, texture_manager);
  ASSERT_TRUE(result);
  EXPECT_EQ(16776960u, result->SamplesPerPixel());
}