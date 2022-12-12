#include "frontends/pbrt/samplers/parse.h"

#include "googletest/include/gtest/gtest.h"

TEST(Parse, TooFewParameters) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EXIT(iris::pbrt_frontend::samplers::Parse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few parameters to directive: Sampler");
}

TEST(Parse, NotAString) {
  std::stringstream input("1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EXIT(iris::pbrt_frontend::samplers::Parse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Parameter to Sampler must be a string");
}

TEST(Parse, InvalidType) {
  std::stringstream input("\"NotAType\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EXIT(iris::pbrt_frontend::samplers::Parse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Unsupported type for directive Sampler: NotAType");
}

TEST(Parse, Random) {
  std::stringstream input("\"random\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::samplers::Parse(tokenizer);
}

TEST(Parse, Stratified) {
  std::stringstream input("\"stratified\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::samplers::Parse(tokenizer);
}