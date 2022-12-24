#include "frontends/pbrt/film/image.h"

#include "frontends/pbrt/build_objects.h"
#include "frontends/pbrt/spectrum_managers/test_spectrum_manager.h"
#include "googletest/include/gtest/gtest.h"

TEST(Image, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::film::g_image_builder, tokenizer, spectrum_manager,
      texture_manager);
  EXPECT_EQ("pbrt.pfm", result.filename);
  EXPECT_EQ(480u, result.resolution.first);
  EXPECT_EQ(640u, result.resolution.second);
  EXPECT_EQ(0.0, result.crop_window[0]);
  EXPECT_EQ(1.0, result.crop_window[1]);
  EXPECT_EQ(0.0, result.crop_window[2]);
  EXPECT_EQ(1.0, result.crop_window[3]);
  EXPECT_EQ(35.0, result.diagonal);
  EXPECT_FALSE(result.max_sample_luminance);
}

TEST(Image, NegativeCropWindow0) {
  std::stringstream input("\"float cropwindow\" [-1.0 1.0 0.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: cropwindow");
}

TEST(Image, NegativeCropWindow1) {
  std::stringstream input("\"float cropwindow\" [0.0 -1.0 0.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: cropwindow");
}

TEST(Image, NegativeCropWindow2) {
  std::stringstream input("\"float cropwindow\" [0.0 1.0 -1.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: cropwindow");
}

TEST(Image, NegativeCropWindow3) {
  std::stringstream input("\"float cropwindow\" [0.0 1.0 0.0 -1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: cropwindow");
}

TEST(Image, TooLargeCropWindow0) {
  std::stringstream input("\"float cropwindow\" [2.0 1.0 0.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: cropwindow");
}

TEST(Image, TooLargeCropWindow1) {
  std::stringstream input("\"float cropwindow\" [0.0 2.0 0.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: cropwindow");
}

TEST(Image, TooLargeCropWindow2) {
  std::stringstream input("\"float cropwindow\" [0.0 1.0 2.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: cropwindow");
}

TEST(Image, TooLargeCropWindow3) {
  std::stringstream input("\"float cropwindow\" [0.0 1.0 0.0 2.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: cropwindow");
}

TEST(Image, BadCropWindow0) {
  std::stringstream input("\"float cropwindow\" [0.0 0.0 0.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid values for parameter list: cropwindow");
}

TEST(Image, BadCropWindow1) {
  std::stringstream input("\"float cropwindow\" [0.0 1.0 0.0 0.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid values for parameter list: cropwindow");
}

TEST(Image, Diagonal) {
  std::stringstream input("\"float diagonal\" 0.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: diagonal");
}

TEST(Image, MaxSampleLuminance) {
  std::stringstream input("\"float maxsampleluminance\" 0.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: maxsampleluminance");
}

TEST(Image, Scale) {
  std::stringstream input("\"float scale\" 0.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: scale");
}

TEST(Image, XResolution) {
  std::stringstream input("\"integer x_resolution\" 0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: x_resolution");
}

TEST(Image, YResolution) {
  std::stringstream input("\"integer y_resolution\" 0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range value for parameter: y_resolution");
}

TEST(Image, BadExtension) {
  std::stringstream input("\"string filename\" \"image.txt\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  EXPECT_EXIT(iris::pbrt_frontend::BuildObject(
                  *iris::pbrt_frontend::film::g_image_builder, tokenizer,
                  spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Unsupported file extension for parameter: filename");
}

TEST(Image, AllParameters) {
  std::stringstream input(
      "\"float cropwindow\" [0.25 0.75 0.25 0.75] "
      "\"float diagonal\" 25.0 "
      "\"string filename\" \"iris.pfm\" "
      "\"float maxsampleluminance\" 1.0 "
      "\"float scale\" 2.0 "
      "\"integer x_resolution\" 500 "
      "\"integer y_resolution\" 250");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::film::g_image_builder, tokenizer, spectrum_manager,
      texture_manager);
  EXPECT_EQ("iris.pfm", result.filename);
  EXPECT_EQ(250u, result.resolution.first);
  EXPECT_EQ(500u, result.resolution.second);
  EXPECT_EQ(0.25, result.crop_window[0]);
  EXPECT_EQ(0.75, result.crop_window[1]);
  EXPECT_EQ(0.25, result.crop_window[2]);
  EXPECT_EQ(0.75, result.crop_window[3]);
  EXPECT_EQ(25.0, result.diagonal);
  EXPECT_EQ(1.0, result.max_sample_luminance.value());
}