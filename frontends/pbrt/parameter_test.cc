#include "frontends/pbrt/parameter.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/reflectors/mock_reflector.h"
#include "iris/spectra/mock_spectrum.h"

class TestSpectrumManager final : public iris::pbrt_frontend::SpectrumManager {
 public:
  iris::ReferenceCounted<iris::Spectrum> AllocateSpectrum(
      const std::map<iris::visual, iris::visual> wavelengths) const override {
    EXPECT_FALSE(true);
    return iris::ReferenceCounted<iris::Spectrum>();
  }

  iris::ReferenceCounted<iris::Reflector> AllocateReflector(
      const std::map<iris::visual, iris::visual> wavelengths) const override {
    EXPECT_FALSE(true);
    return iris::ReferenceCounted<iris::Reflector>();
  }

 protected:
  iris::ReferenceCounted<iris::Spectrum> AllocateSpectrum(
      const iris::Color& color) const override {
    return iris::MakeReferenceCounted<iris::spectra::MockSpectrum>();
  }

  iris::ReferenceCounted<iris::Reflector> AllocateReflector(
      const iris::Color& color) const override {
    return iris::MakeReferenceCounted<iris::reflectors::MockReflector>();
  }
};

TEST(Parameter, BoolWrongType) {
  std::stringstream input("\"integer name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  EXPECT_EXIT(
      parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::BOOL,
                         spectrum_manager, texture_manager),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Wrong type for parameter list: name");
}

TEST(Parameter, BoolTooFew) {
  std::stringstream input("\"bool name\" [\"true\" \"true\"]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::BOOL,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetBoolValues(0u, 3u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few values in parameter list: name");
}

TEST(Parameter, BoolTooMany) {
  std::stringstream input("\"bool name\" [\"true\" \"true\"]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::BOOL,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetBoolValues(1u, 1u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too many values in parameter list: name");
}

TEST(Parameter, Bool) {
  std::stringstream input("\"bool name\" [\"true\"]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::BOOL,
                     spectrum_manager, texture_manager);

  ASSERT_EQ(1u, parameter.GetBoolValues(1u, 1u).size());
  EXPECT_TRUE(parameter.GetBoolValues(1u, 1u).at(0));
}