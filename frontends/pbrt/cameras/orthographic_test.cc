#include "frontends/pbrt/cameras/orthographic.h"

#include "frontends/pbrt/build_objects.h"
#include "frontends/pbrt/spectrum_managers/test_spectrum_manager.h"
#include "googletest/include/gtest/gtest.h"

static const iris::pbrt_frontend::MatrixManager::Transformation
    g_transformation = {iris::Matrix::Identity(), iris::Matrix::Identity()};

TEST(Orthographic, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
      spectrum_manager, texture_manager, g_transformation);
  EXPECT_TRUE(result);
}

TEST(Orthographic, AspectRatioNegative) {
  std::stringstream input("\"float frameaspectratio\" -1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
          spectrum_manager, texture_manager, g_transformation),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Out of range value for parameter: frameaspectratio");
}

TEST(Orthographic, AspectRatioZero) {
  std::stringstream input("\"float frameaspectratio\" 0.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
          spectrum_manager, texture_manager, g_transformation),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Out of range value for parameter: frameaspectratio");
}

TEST(Orthographic, ScreenWindowTooFew) {
  std::stringstream input("\"float screenwindow\" [0.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
          spectrum_manager, texture_manager, g_transformation),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Too few values in parameter list: screenwindow");
}

TEST(Orthographic, ScreenWindowTooMany) {
  std::stringstream input("\"float screenwindow\" [0.0 1.0 0.0 1.0 0.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
          spectrum_manager, texture_manager, g_transformation),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Too many values in parameter list: screenwindow");
}

TEST(Orthographic, ScreenWindowBadXBounds) {
  std::stringstream input("\"float screenwindow\" [-1.0 -1.0 -2.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
          spectrum_manager, texture_manager, g_transformation),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Invalid bounds from parameter: screenwindow");
}

TEST(Orthographic, ScreenWindowBadXBoundsEqual) {
  std::stringstream input("\"float screenwindow\" [-1.0 -1.0 -1.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
          spectrum_manager, texture_manager, g_transformation),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Invalid bounds from parameter: screenwindow");
}

TEST(Orthographic, ScreenWindowBadYBounds) {
  std::stringstream input("\"float screenwindow\" [-1.0 -1.0 1.0 -2.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
          spectrum_manager, texture_manager, g_transformation),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Invalid bounds from parameter: screenwindow");
}

TEST(Orthographic, ScreenWindowBadYBoundsEqual) {
  std::stringstream input("\"float screenwindow\" [-1.0 -1.0 1.0 -1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
          spectrum_manager, texture_manager, g_transformation),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Invalid bounds from parameter: screenwindow");
}

TEST(Orthographic, BothSpecified) {
  std::stringstream input(
      "\"float frameaspectratio\" 1.0 \"float screenwindow\" [-1.0 -1.0 1.0 "
      "1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EXIT(
      iris::pbrt_frontend::BuildObject(
          *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
          spectrum_manager, texture_manager, g_transformation),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Cannot specify parameters together: frameaspectratio, "
      "screenwindow");
}

TEST(Orthographic, WithWindow) {
  std::stringstream input("\"float screenwindow\" [-2.0 -2.0 2.0 2.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
      spectrum_manager, texture_manager, g_transformation);
  ASSERT_TRUE(result);

  auto camera = result(std::make_pair(1u, 1u));
  auto top_left = camera->Compute({0.0, 0.0}, nullptr);
  EXPECT_EQ(iris::Point(-2.0, 2.0, 0.0), top_left.origin);
  auto bottom_right = camera->Compute({1.0, 1.0}, nullptr);
  EXPECT_EQ(iris::Point(2.0, -2.0, 0.0), bottom_right.origin);
}

TEST(Orthographic, WithXAspectRatio) {
  std::stringstream input("\"float frameaspectratio\" 2.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
      spectrum_manager, texture_manager, g_transformation);
  ASSERT_TRUE(result);

  auto camera = result(std::make_pair(1u, 1u));
  auto top_left = camera->Compute({0.0, 0.0}, nullptr);
  EXPECT_EQ(iris::Point(-2.0, 1.0, 0.0), top_left.origin);
  auto bottom_right = camera->Compute({1.0, 1.0}, nullptr);
  EXPECT_EQ(iris::Point(2.0, -1.0, 0.0), bottom_right.origin);
}

TEST(Orthographic, WithYAspectRatio) {
  std::stringstream input("\"float frameaspectratio\" 0.5");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
      spectrum_manager, texture_manager, g_transformation);
  ASSERT_TRUE(result);

  auto camera = result(std::make_pair(1u, 1u));
  auto top_left = camera->Compute({0.0, 0.0}, nullptr);
  EXPECT_EQ(iris::Point(-1.0, 2.0, 0.0), top_left.origin);
  auto bottom_right = camera->Compute({1.0, 1.0}, nullptr);
  EXPECT_EQ(iris::Point(1.0, -2.0, 0.0), bottom_right.origin);
}

TEST(Orthographic, FromFrameX) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
      spectrum_manager, texture_manager, g_transformation);
  ASSERT_TRUE(result);

  auto camera = result(std::make_pair(1u, 2u));
  auto top_left = camera->Compute({0.0, 0.0}, nullptr);
  EXPECT_EQ(iris::Point(-2.0, 1.0, 0.0), top_left.origin);
  auto bottom_right = camera->Compute({1.0, 1.0}, nullptr);
  EXPECT_EQ(iris::Point(2.0, -1.0, 0.0), bottom_right.origin);
}

TEST(Orthographic, FromFrameY) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::cameras::g_orthographic_builder, tokenizer,
      spectrum_manager, texture_manager, g_transformation);
  ASSERT_TRUE(result);

  auto camera = result(std::make_pair(2u, 1u));
  auto top_left = camera->Compute({0.0, 0.0}, nullptr);
  EXPECT_EQ(iris::Point(-1.0, 2.0, 0.0), top_left.origin);
  auto bottom_right = camera->Compute({1.0, 1.0}, nullptr);
  EXPECT_EQ(iris::Point(1.0, -2.0, 0.0), bottom_right.origin);
}