#include "frontends/pbrt/integrators/pixel_bounds.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/reflectors/mock_reflector.h"
#include "iris/spectra/mock_spectrum.h"

class TestSpectrumManager final : public iris::pbrt_frontend::SpectrumManager {
 public:
  iris::ReferenceCounted<iris::Spectrum> AllocateSpectrum(
      const std::map<iris::visual, iris::visual>& wavelengths) override {
    return iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();
  }
  iris::ReferenceCounted<iris::Spectrum> AllocateSpectrum(
      const iris::pbrt_frontend::Color& color) override {
    return iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();
  }

  iris::ReferenceCounted<iris::Reflector> AllocateReflector(
      const std::map<iris::visual, iris::visual>& wavelengths) override {
    return iris::MakeReferenceCounted<iris::reflectors::MockReflector>();
  }

  iris::ReferenceCounted<iris::Reflector> AllocateReflector(
      const iris::pbrt_frontend::Color& color) override {
    return iris::MakeReferenceCounted<iris::reflectors::MockReflector>();
  }

  void Clear() override {}
};

TEST(ParseLightScene, NegativeXBegin) {
  std::stringstream input("\"integer pixelbounds\" [-1 2 3 4]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(iris::pbrt_frontend::integrators::ParsePixelBounds(parameter),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Negative value in parameter list: pixelbounds");
}

TEST(ParseLightScene, NegativeXEnd) {
  std::stringstream input("\"integer pixelbounds\" [1 -2 3 4]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(iris::pbrt_frontend::integrators::ParsePixelBounds(parameter),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Negative value in parameter list: pixelbounds");
}

TEST(ParseLightScene, NegativeYBegin) {
  std::stringstream input("\"integer pixelbounds\" [1 2 -3 4]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(iris::pbrt_frontend::integrators::ParsePixelBounds(parameter),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Negative value in parameter list: pixelbounds");
}

TEST(ParseLightScene, NegativeYEnd) {
  std::stringstream input("\"integer pixelbounds\" [1 2 3 -4]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(iris::pbrt_frontend::integrators::ParsePixelBounds(parameter),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Negative value in parameter list: pixelbounds");
}

TEST(ParseLightScene, BadOrderX) {
  std::stringstream input("\"integer pixelbounds\" [2 1 3 4]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(iris::pbrt_frontend::integrators::ParsePixelBounds(parameter),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid values for parameter list: pixelbounds");
}

TEST(ParseLightScene, BadOrderY) {
  std::stringstream input("\"integer pixelbounds\" [1 2 4 3]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(iris::pbrt_frontend::integrators::ParsePixelBounds(parameter),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid values for parameter list: pixelbounds");
}

TEST(ParseLightScene, Succeeds) {
  std::stringstream input("\"integer pixelbounds\" [1 2 3 4]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  auto values = iris::pbrt_frontend::integrators::ParsePixelBounds(parameter);
  EXPECT_EQ(1u, values.at(0));
  EXPECT_EQ(2u, values.at(1));
  EXPECT_EQ(3u, values.at(2));
  EXPECT_EQ(4u, values.at(3));
}