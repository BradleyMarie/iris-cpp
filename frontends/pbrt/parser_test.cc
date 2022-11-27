#include "frontends/pbrt/parser.h"

#include "googletest/include/gtest/gtest.h"

TEST(Parser, InvalidDirective) {
  std::stringstream input("NotADirective");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::Parser parser;
  EXPECT_EXIT(parser.ParseFrom(".", tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid directive: NotADirective");
}