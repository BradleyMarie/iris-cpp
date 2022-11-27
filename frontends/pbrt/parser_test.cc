#include "frontends/pbrt/parser.h"

#include <cstdlib>

#include "googletest/include/gtest/gtest.h"

TEST(Parser, InvalidDirective) {
  std::stringstream input("NotADirective");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser;
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid directive: NotADirective");
}

TEST(Include, MissingToken) {
  std::stringstream input("Include");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser;
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few parameters to directive Include");
}

TEST(Include, NotQuoted) {
  std::stringstream input("Include 2.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser;
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Parameter to Include must be a string");
}

TEST(Include, Circular) {
  std::stringstream input(
      "Include \"frontends/pbrt/test_data/include_circular_first.pbrt\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser;
  EXPECT_EXIT(
      parser.ParseFrom(".", tokenizer), testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Detected cyclic Include of file: include_circular_first.pbrt");
}

TEST(Include, CircularSelf) {
  std::stringstream input(
      "Include \"frontends/pbrt/test_data/include_circular_self.pbrt\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser;
  EXPECT_EXIT(
      parser.ParseFrom(".", tokenizer), testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Detected cyclic Include of file: include_circular_self.pbrt");
}

TEST(Include, Empty) {
  std::stringstream input(
      "Include \"frontends/pbrt/test_data/include_empty.pbrt\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser;
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Final directive should be WorldEnd");
}