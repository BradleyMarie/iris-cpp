#include "frontends/pbrt/tokenizer.h"

#include <sstream>

#include "googletest/include/gtest/gtest.h"

TEST(Tokenizer, Empty) {
  std::stringstream input;
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_FALSE(tokenizer.Peek());
  EXPECT_FALSE(tokenizer.Next());
}

TEST(Tokenizer, MoveConstruct) {
  std::stringstream input("hello");
  iris::pbrt_frontend::Tokenizer tokenizer0(input);
  EXPECT_EQ("hello", tokenizer0.Peek());

  iris::pbrt_frontend::Tokenizer tokenizer1(std::move(tokenizer0));
  EXPECT_FALSE(tokenizer0.Peek());
  EXPECT_FALSE(tokenizer0.Next());
  EXPECT_EQ("hello", tokenizer1.Peek());
  EXPECT_EQ("hello", tokenizer1.Next());
}

TEST(Tokenizer, Move) {
  std::stringstream input("hello");
  iris::pbrt_frontend::Tokenizer tokenizer0(input);
  EXPECT_EQ("hello", tokenizer0.Peek());

  std::stringstream empty;
  iris::pbrt_frontend::Tokenizer tokenizer1(empty);
  tokenizer1 = std::move(tokenizer0);

  EXPECT_FALSE(tokenizer0.Peek());
  EXPECT_FALSE(tokenizer0.Next());
  EXPECT_EQ("hello", tokenizer1.Peek());
  EXPECT_EQ("hello", tokenizer1.Next());
}

TEST(Tokenizer, QuotedString) {
  std::stringstream input("\"hello world!\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EQ("\"hello world!\"", tokenizer.Next());
}

TEST(Tokenizer, ValidEscapeCodes) {
  std::string escaped_characters[8] = {"b", "f",  "n", "r",
                                       "t", "\\", "'", "\""};
  std::string escaped_values[8] = {"\b", "\f", "\n", "\r",
                                   "\t", "\\", "'",  "\""};
  for (size_t i = 0; i < 8; i++) {
    std::string contents = "\"\\" + escaped_characters[i] + "\"";
    std::stringstream input(contents);
    iris::pbrt_frontend::Tokenizer tokenizer(input);
    EXPECT_EQ("\"" + escaped_values[i] + "\"", tokenizer.Next());
  }
}

TEST(Tokenizer, IllegalEscape) {
  std::stringstream input("\"\\!\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EXIT(tokenizer.Next(), testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Illegal escape character: \\\\!");
}

TEST(Tokenizer, IllegalNewline) {
  std::stringstream input("\"\n\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EXIT(tokenizer.Next(), testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: New line found before end of quoted string");
}

TEST(Tokenizer, UnterminatedQuote) {
  std::stringstream input("\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EXIT(tokenizer.Next(), testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Unterminated quoted string");
}

TEST(Tokenizer, IgnoresComments) {
  std::stringstream input("#ignored one\n#ignored two\nAbc");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EQ("Abc", tokenizer.Next());
}

TEST(Tokenizer, Array) {
  std::stringstream input("[]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EQ("[", tokenizer.Next());
  EXPECT_EQ("]", tokenizer.Next());
}

TEST(Tokenizer, MultipleTokens) {
  std::stringstream input("Token1 [1.0] Two \"hello world\" [3] [\"a\"]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  EXPECT_EQ("Token1", tokenizer.Peek());
  EXPECT_EQ("Token1", tokenizer.Peek());
  EXPECT_EQ("Token1", tokenizer.Next());
  EXPECT_EQ("[", tokenizer.Peek());
  EXPECT_EQ("[", tokenizer.Next());
  EXPECT_EQ("1.0", tokenizer.Peek());
  EXPECT_EQ("1.0", tokenizer.Next());
  EXPECT_EQ("]", tokenizer.Peek());
  EXPECT_EQ("]", tokenizer.Next());
  EXPECT_EQ("Two", tokenizer.Peek());
  EXPECT_EQ("Two", tokenizer.Next());
  EXPECT_EQ("\"hello world\"", tokenizer.Peek());
  EXPECT_EQ("\"hello world\"", tokenizer.Next());
  EXPECT_EQ("[", tokenizer.Peek());
  EXPECT_EQ("[", tokenizer.Next());
  EXPECT_EQ("3", tokenizer.Peek());
  EXPECT_EQ("3", tokenizer.Next());
  EXPECT_EQ("]", tokenizer.Peek());
  EXPECT_EQ("]", tokenizer.Next());
  EXPECT_EQ("[", tokenizer.Peek());
  EXPECT_EQ("[", tokenizer.Next());
  EXPECT_EQ("\"a\"", tokenizer.Peek());
  EXPECT_EQ("\"a\"", tokenizer.Next());
  EXPECT_EQ("]", tokenizer.Peek());
  EXPECT_EQ("]", tokenizer.Next());
  EXPECT_EQ(std::nullopt, tokenizer.Peek());
  EXPECT_EQ(std::nullopt, tokenizer.Next());
}