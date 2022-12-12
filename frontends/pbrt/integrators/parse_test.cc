#include "frontends/pbrt/integrators/parse.h"

#include "googletest/include/gtest/gtest.h"

TEST(Parse, TooFewParameters) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EXIT(iris::pbrt_frontend::integrators::Parse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few parameters to directive: Integrator");
}

TEST(Parse, NotAString) {
  std::stringstream input("1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EXIT(iris::pbrt_frontend::integrators::Parse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Parameter to Integrator must be a string");
}

TEST(Parse, InvalidType) {
  std::stringstream input("\"NotAType\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EXIT(iris::pbrt_frontend::integrators::Parse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Unsupported type for directive Integrator: NotAType");
}

TEST(Parse, Path) {
  std::stringstream input("\"path\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::integrators::Parse(tokenizer);
}