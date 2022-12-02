#include "frontends/pbrt/parser.h"

#include <cstdlib>

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

TEST(Parser, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_FALSE(parser.ParseFrom(".", tokenizer));
}

TEST(Parser, NoEnd) {
  std::stringstream input("WorldBegin");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Final directive should be WorldEnd");
}

TEST(Parser, InvalidDirective) {
  std::stringstream input("NotADirective");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid directive: NotADirective");
}

TEST(Include, MissingToken) {
  std::stringstream input("Include");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few parameters to directive Include");
}

TEST(Include, NotQuoted) {
  std::stringstream input("Include 2.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Parameter to Include must be a string");
}

TEST(Include, Circular) {
  std::stringstream input(
      "Include \"frontends/pbrt/test_data/include_circular_first.pbrt\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(
      parser.ParseFrom(".", tokenizer), testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Detected cyclic Include of file: include_circular_first.pbrt");
}

TEST(Include, CircularSelf) {
  std::stringstream input(
      "Include \"frontends/pbrt/test_data/include_circular_self.pbrt\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(
      parser.ParseFrom(".", tokenizer), testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Detected cyclic Include of file: include_circular_self.pbrt");
}

TEST(Include, Empty) {
  std::stringstream input(
      "Include \"frontends/pbrt/test_data/include_empty.pbrt\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Final directive should be WorldEnd");
}

TEST(Integrator, AfterWorldBegin) {
  std::stringstream input("WorldBegin Integrator");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Directive cannot be specified between WorldBegin and "
              "WorldEnd: Integrator");
}

TEST(Integrator, Duplicate) {
  std::stringstream input("Integrator \"path\" Integrator \"path\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "Directive specified twice for a render: Integrator");
}

TEST(Matrix, Parses) {
  std::stringstream input("Identity");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Final directive should be WorldEnd");
}

TEST(PixelFilter, AfterWorldBegin) {
  std::stringstream input("WorldBegin PixelFilter");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Directive cannot be specified between WorldBegin and "
              "WorldEnd: PixelFilter");
}

TEST(PixelFilter, MissingToken) {
  std::stringstream input("PixelFilter");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few parameters to directive PixelFilter");
}

TEST(PixelFilter, NotQuoted) {
  std::stringstream input("PixelFilter 2.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Parameter to PixelFilter must be a string");
}

TEST(PixelFilter, Duplicate) {
  std::stringstream input("PixelFilter \"box\" PixelFilter \"box\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "Directive specified twice for a render: PixelFilter");
}

TEST(Sampler, AfterWorldBegin) {
  std::stringstream input("WorldBegin Sampler");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Directive cannot be specified between WorldBegin and "
              "WorldEnd: Sampler");
}

TEST(Sampler, Duplicate) {
  std::stringstream input("Sampler \"random\" Sampler \"random\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "Directive specified twice for a render: Sampler");
}

TEST(WorldBegin, Duplicate) {
  std::stringstream input("WorldBegin WorldBegin");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid WorldBegin directive");
}

TEST(WorldEnd, NoWorldBegin) {
  std::stringstream input("WorldEnd");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser(std::make_unique<TestSpectrumManager>());
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid WorldEnd directive");
}