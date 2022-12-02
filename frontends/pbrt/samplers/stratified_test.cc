#include "frontends/pbrt/samplers/stratified.h"

#include "frontends/pbrt/build_objects.h"
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

TEST(Stratified, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  TestSpectrumManager spectrum_manager;
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

  TestSpectrumManager spectrum_manager;
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

  TestSpectrumManager spectrum_manager;
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

  TestSpectrumManager spectrum_manager;
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

  TestSpectrumManager spectrum_manager;
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

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;

  auto result = iris::pbrt_frontend::BuildObject(
      *iris::pbrt_frontend::samplers::g_stratified_builder, tokenizer,
      spectrum_manager, texture_manager);
  ASSERT_TRUE(result);
  EXPECT_EQ(16776960u, result->SamplesPerPixel());
}