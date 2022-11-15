#include "frontends/pbrt/parameter_list.h"

#include "googletest/include/gtest/gtest.h"

TEST(ParameterList, Empty) {
  std::stringstream input;
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_FALSE(parameter_list.ParseFrom(tokenizer));
}

TEST(ParameterList, NotQuoted) {
  std::stringstream input("hello");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_FALSE(parameter_list.ParseFrom(tokenizer));
}

TEST(ParameterList, QuotedOneWord) {
  std::stringstream input("\"hello\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_FALSE(parameter_list.ParseFrom(tokenizer));
}

TEST(ParameterList, QuotedLeadingSpace) {
  std::stringstream input("\" hello\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_FALSE(parameter_list.ParseFrom(tokenizer));
}

TEST(ParameterList, QuotedOneWordWithOneSpace) {
  std::stringstream input("\"hello \"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_FALSE(parameter_list.ParseFrom(tokenizer));
}

TEST(ParameterList, QuotedOneWordWithMultipleSpaces) {
  std::stringstream input("\"hello         \"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_FALSE(parameter_list.ParseFrom(tokenizer));
}

TEST(ParameterList, QuotedTwoWordsTrailingSpaces) {
  std::stringstream input("\"hello world \"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_FALSE(parameter_list.ParseFrom(tokenizer));
}

TEST(ParameterList, QuotedThreeWorlds) {
  std::stringstream input("\"hello big world\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_FALSE(parameter_list.ParseFrom(tokenizer));
}

TEST(ParameterList, QuotedInvalidType) {
  std::stringstream input("\"hello world\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_FALSE(parameter_list.ParseFrom(tokenizer));
}

TEST(ParameterList, UnclosedArray) {
  std::stringstream input("\"bool name\" [");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Unterminated parameter list");
}

TEST(ParameterList, EmptyList) {
  std::stringstream input("\"bool name\" []");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Empty parameter list");
}

TEST(ParameterList, ParseError) {
  std::stringstream input("\"bool name\" [notabool]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Failed to parse bool value: notabool");
}

TEST(ParameterList, SingleValue) {
  std::stringstream input("\"bool name\" \"true\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ("bool", parameter_list.GetTypeName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::BOOL, parameter_list.GetType());
  ASSERT_EQ(1u, parameter_list.GetBoolValues().size());
  EXPECT_TRUE(parameter_list.GetBoolValues().at(0));
}

TEST(ParameterList, Bool) {
  std::stringstream input("\"bool name\" [\"true\" \"false\"]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::BOOL, parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetBoolValues().size());
  EXPECT_TRUE(parameter_list.GetBoolValues().at(0));
  EXPECT_FALSE(parameter_list.GetBoolValues().at(1));
}

TEST(ParameterList, Color) {
  std::stringstream input("\"color name\" [1 2 16 3 2 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::COLOR,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetColorValues().size());

  auto color0 = parameter_list.GetColorValues().at(0);
  EXPECT_EQ(1, color0.values[0]);
  EXPECT_EQ(2, color0.values[1]);
  EXPECT_EQ(16, color0.values[2]);
  EXPECT_EQ(iris::pbrt_frontend::Color::RGB, color0.space);

  auto color1 = parameter_list.GetColorValues().at(1);
  EXPECT_EQ(3, color1.values[0]);
  EXPECT_EQ(2, color1.values[1]);
  EXPECT_EQ(1, color1.values[2]);
  EXPECT_EQ(iris::pbrt_frontend::Color::RGB, color0.space);
}

TEST(ParameterList, FloatNaN) {
  std::stringstream input("\"float name\" [NaN]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Failed to parse float value: NaN");
}

TEST(ParameterList, FloatInf) {
  std::stringstream input("\"float name\" [1.7e9999999]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Failed to parse float value: 1.7e9999999");
}

TEST(ParameterList, Float) {
  std::stringstream input("\"float name\" [0.0 1.0 1.7e9]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::FLOAT,
            parameter_list.GetType());
  ASSERT_EQ(3u, parameter_list.GetFloatValues().size());
  EXPECT_EQ(0.0, parameter_list.GetFloatValues().at(0));
  EXPECT_EQ(1.0, parameter_list.GetFloatValues().at(1));
  EXPECT_EQ(1.7e9, parameter_list.GetFloatValues().at(2));
}

TEST(ParameterList, IntegerTooLarge) {
  std::stringstream input("\"integer name\" [18446744073709551616]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Failed to parse integer value: 18446744073709551616");
}

TEST(ParameterList, Integer) {
  std::stringstream input("\"integer name\" [1 2 16]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::INTEGER,
            parameter_list.GetType());
  ASSERT_EQ(3u, parameter_list.GetIntegerValues().size());
  EXPECT_EQ(1, parameter_list.GetIntegerValues().at(0));
  EXPECT_EQ(2, parameter_list.GetIntegerValues().at(1));
  EXPECT_EQ(16, parameter_list.GetIntegerValues().at(2));
}

TEST(ParameterList, Normal) {
  std::stringstream input("\"normal name\" [1 2 16 3 2 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::NORMAL,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetNormalValues().size());
  EXPECT_EQ(iris::Vector(1, 2, 16), parameter_list.GetNormalValues().at(0));
  EXPECT_EQ(iris::Vector(3, 2, 1), parameter_list.GetNormalValues().at(1));
}

TEST(ParameterList, Point) {
  std::stringstream input("\"point name\" [1 2 16 3 2 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::POINT3,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetPoint3Values().size());
  EXPECT_EQ(iris::Point(1, 2, 16), parameter_list.GetPoint3Values().at(0));
  EXPECT_EQ(iris::Point(3, 2, 1), parameter_list.GetPoint3Values().at(1));
}

TEST(ParameterList, Point3WrongNumber) {
  std::stringstream input("\"point3 name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The number of values in a point3 parameter list must be "
              "evenly divisible by 3");
}

TEST(ParameterList, Point3OutOfRangeX) {
  std::stringstream input("\"point3 name\" [3e39 1 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A point3 parameter list value was out of range");
}

TEST(ParameterList, Point3OutOfRangeY) {
  std::stringstream input("\"point3 name\" [1 3e39 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A point3 parameter list value was out of range");
}

TEST(ParameterList, Point3OutOfRangeZ) {
  std::stringstream input("\"point3 name\" [1 2 3e39]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A point3 parameter list value was out of range");
}

TEST(ParameterList, Point3) {
  std::stringstream input("\"point3 name\" [1 2 16 3 2 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::POINT3,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetPoint3Values().size());
  EXPECT_EQ(iris::Point(1, 2, 16), parameter_list.GetPoint3Values().at(0));
  EXPECT_EQ(iris::Point(3, 2, 1), parameter_list.GetPoint3Values().at(1));
}

TEST(ParameterList, RgbWrongNumber) {
  std::stringstream input("\"rgb name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The number of values in an rgb parameter list must be "
              "evenly divisible by 3");
}

TEST(ParameterList, RgbNegativeX) {
  std::stringstream input("\"rgb name\" [1 1 -1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The values in an rgb parameter list cannot be negative");
}

TEST(ParameterList, RgbNegativeY) {
  std::stringstream input("\"rgb name\" [1 -1 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The values in an rgb parameter list cannot be negative");
}

TEST(ParameterList, RgbNegativeZ) {
  std::stringstream input("\"rgb name\" [1 2 -1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The values in an rgb parameter list cannot be negative");
}

TEST(ParameterList, RgbRangeX) {
  std::stringstream input("\"rgb name\" [3e39 1 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A rgb parameter list value was out of range");
}

TEST(ParameterList, RgbRangeY) {
  std::stringstream input("\"rgb name\" [1 3e39 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A rgb parameter list value was out of range");
}

TEST(ParameterList, RgbRangeZ) {
  std::stringstream input("\"rgb name\" [1 2 3e39]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A rgb parameter list value was out of range");
}

TEST(ParameterList, Rgb) {
  std::stringstream input("\"rgb name\" [1 2 16 3 2 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::COLOR,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetColorValues().size());

  auto color0 = parameter_list.GetColorValues().at(0);
  EXPECT_EQ(1, color0.values[0]);
  EXPECT_EQ(2, color0.values[1]);
  EXPECT_EQ(16, color0.values[2]);
  EXPECT_EQ(iris::pbrt_frontend::Color::RGB, color0.space);

  auto color1 = parameter_list.GetColorValues().at(1);
  EXPECT_EQ(3, color1.values[0]);
  EXPECT_EQ(2, color1.values[1]);
  EXPECT_EQ(1, color1.values[2]);
  EXPECT_EQ(iris::pbrt_frontend::Color::RGB, color0.space);
}

TEST(ParameterList, SpectrumWrongNumber) {
  std::stringstream input("\"spectrum name\" [1 2 3]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(
      parameter_list.ParseFrom(tokenizer),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: The number of values in a spectrum parameter list cannot be odd");
}

TEST(ParameterList, SpectrumNegativeWavelength) {
  std::stringstream input("\"spectrum name\" [-1 0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(
      parameter_list.ParseFrom(tokenizer),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: The values in an spectrum parameter list cannot be negative");
}

TEST(ParameterList, SpectrumNegativeIntensity) {
  std::stringstream input("\"spectrum name\" [1 -1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(
      parameter_list.ParseFrom(tokenizer),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: The values in an spectrum parameter list cannot be negative");
}

TEST(ParameterList, SpectrumNumericOutOfRangeWavelength) {
  std::stringstream input("\"spectrum name\" [3e39 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A spectrum parameter list value was out of range");
}

TEST(ParameterList, SpectrumNumericOutOfRangeIntensity) {
  std::stringstream input("\"spectrum name\" [1 3e39]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A spectrum parameter list value was out of range");
}

TEST(ParameterList, SpectrumDuplicateWavelengths) {
  std::stringstream input("\"spectrum name\" [1 2 1 3]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(
      parameter_list.ParseFrom(tokenizer),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: A spectrum parameter list contained duplicate wavelengths");
}

TEST(ParameterList, Spectrum) {
  std::stringstream input("\"spectrum name\" [1 2 3 4]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::SPECTRUM,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetSpectrumValues().size());
  ASSERT_EQ(2, parameter_list.GetSpectrumValues().at(1));
  ASSERT_EQ(4, parameter_list.GetSpectrumValues().at(3));
}

TEST(ParameterList, StringNotQuoted) {
  std::stringstream input("\"string name\" [abc]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Failed to parse string value: abc");
}

TEST(ParameterList, String) {
  std::stringstream input("\"string name\" [\"abc\" \"123\"]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::STRING,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetStringValues().size());
  EXPECT_EQ("abc", parameter_list.GetStringValues().at(0));
  EXPECT_EQ("123", parameter_list.GetStringValues().at(1));
}

TEST(ParameterList, Texture) {
  std::stringstream input("\"texture name\" [\"abc\" \"123\"]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::TEXTURE,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetTextureValues().size());
  EXPECT_EQ("abc", parameter_list.GetTextureValues().at(0));
  EXPECT_EQ("123", parameter_list.GetTextureValues().at(1));
}

TEST(ParameterList, Vector) {
  std::stringstream input("\"vector name\" [1 2 16 3 2 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::VECTOR3,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetVector3Values().size());
  EXPECT_EQ(iris::Vector(1, 2, 16), parameter_list.GetVector3Values().at(0));
  EXPECT_EQ(iris::Vector(3, 2, 1), parameter_list.GetVector3Values().at(1));
}

TEST(ParameterList, Vector3WrongNumber) {
  std::stringstream input("\"vector3 name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The number of values in a vector3 parameter list must be "
              "evenly divisible by 3");
}

TEST(ParameterList, Vector3AllZero) {
  std::stringstream input("\"vector3 name\" [0 0 0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A vector3 parameter list must contain at least one "
              "non-zero value for each output vector");
}

TEST(ParameterList, Vector3OutOfRangeX) {
  std::stringstream input("\"vector3 name\" [3e39 1 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A vector3 parameter list value was out of range");
}

TEST(ParameterList, Vector3OutOfRangeY) {
  std::stringstream input("\"vector3 name\" [1 3e39 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A vector3 parameter list value was out of range");
}

TEST(ParameterList, Vector3OutOfRangeZ) {
  std::stringstream input("\"vector3 name\" [1 2 3e39]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A vector3 parameter list value was out of range");
}

TEST(ParameterList, Vector3) {
  std::stringstream input("\"vector3 name\" [1 2 16 3 2 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::VECTOR3,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetVector3Values().size());
  EXPECT_EQ(iris::Vector(1, 2, 16), parameter_list.GetVector3Values().at(0));
  EXPECT_EQ(iris::Vector(3, 2, 1), parameter_list.GetVector3Values().at(1));
}

TEST(ParameterList, XyzWrongNumber) {
  std::stringstream input("\"xyz name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The number of values in an xyz parameter list must be "
              "evenly divisible by 3");
}

TEST(ParameterList, XyzNegativeX) {
  std::stringstream input("\"xyz name\" [-1 1 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The values in an xyz parameter list cannot be negative");
}

TEST(ParameterList, XyzNegativeY) {
  std::stringstream input("\"xyz name\" [1 -1 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The values in an xyz parameter list cannot be negative");
}

TEST(ParameterList, XyzNegativeZ) {
  std::stringstream input("\"xyz name\" [1 2 -1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: The values in an xyz parameter list cannot be negative");
}

TEST(ParameterList, XyzRangeX) {
  std::stringstream input("\"xyz name\" [3e39 1 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A xyz parameter list value was out of range");
}

TEST(ParameterList, XyzRangeY) {
  std::stringstream input("\"xyz name\" [1 3e39 2]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A xyz parameter list value was out of range");
}

TEST(ParameterList, XyzRangeZ) {
  std::stringstream input("\"xyz name\" [1 2 3e39]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_EXIT(parameter_list.ParseFrom(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A xyz parameter list value was out of range");
}

TEST(ParameterList, Xyz) {
  std::stringstream input("\"xyz name\" [1 2 16 3 2 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer));
  EXPECT_EQ("name", parameter_list.GetName());
  EXPECT_EQ(iris::pbrt_frontend::ParameterList::COLOR,
            parameter_list.GetType());
  ASSERT_EQ(2u, parameter_list.GetColorValues().size());

  auto color0 = parameter_list.GetColorValues().at(0);
  EXPECT_EQ(1, color0.values[0]);
  EXPECT_EQ(2, color0.values[1]);
  EXPECT_EQ(16, color0.values[2]);
  EXPECT_EQ(iris::pbrt_frontend::Color::XYZ, color0.space);

  auto color1 = parameter_list.GetColorValues().at(1);
  EXPECT_EQ(3, color1.values[0]);
  EXPECT_EQ(2, color1.values[1]);
  EXPECT_EQ(1, color1.values[2]);
  EXPECT_EQ(iris::pbrt_frontend::Color::XYZ, color0.space);
}