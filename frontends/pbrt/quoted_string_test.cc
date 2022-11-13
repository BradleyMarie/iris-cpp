#include "frontends/pbrt/quoted_string.h"

#include "googletest/include/gtest/gtest.h"

TEST(Unquote, TooShort) {
  EXPECT_FALSE(iris::pbrt_frontend::Unquote(""));
  EXPECT_FALSE(iris::pbrt_frontend::Unquote("\""));
}

TEST(Unquote, BadFormat) {
  EXPECT_FALSE(iris::pbrt_frontend::Unquote("abc"));
  EXPECT_FALSE(iris::pbrt_frontend::Unquote("\"abc"));
  EXPECT_FALSE(iris::pbrt_frontend::Unquote("abc\""));
  EXPECT_FALSE(iris::pbrt_frontend::Unquote("abc"));
}

TEST(Unquote, Succeeds) {
  EXPECT_EQ("", iris::pbrt_frontend::Unquote("\"\""));
  EXPECT_EQ("abc", iris::pbrt_frontend::Unquote("\"abc\""));
}