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

TEST(Parameter, FloatWrongType) {
  std::stringstream input("\"integer name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  EXPECT_EXIT(
      parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::FLOAT,
                         spectrum_manager, texture_manager),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Wrong type for parameter list: name");
}

TEST(Parameter, FloatTooFew) {
  std::stringstream input("\"float name\" [1.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::FLOAT,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetFloatValues(0u, 3u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few values in parameter list: name");
}

TEST(Parameter, FloatTooMany) {
  std::stringstream input("\"float name\" [1.0 1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::FLOAT,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetFloatValues(1u, 1u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too many values in parameter list: name");
}

TEST(Parameter, Float) {
  std::stringstream input("\"float name\" [1.0]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::FLOAT,
                     spectrum_manager, texture_manager);

  ASSERT_EQ(1u, parameter.GetFloatValues(1u, 1u).size());
  EXPECT_EQ(1.0, parameter.GetFloatValues(1u, 1u).at(0));
}

// FLOAT_TEXTURE

TEST(Parameter, IntegerWrongType) {
  std::stringstream input("\"float name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  EXPECT_EXIT(parameter.LoadFrom(parameter_list,
                                 iris::pbrt_frontend::Parameter::INTEGER,
                                 spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Wrong type for parameter list: name");
}

TEST(Parameter, IntegerTooFew) {
  std::stringstream input("\"integer name\" [1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetIntegerValues(0u, 3u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few values in parameter list: name");
}

TEST(Parameter, IntegerTooMany) {
  std::stringstream input("\"integer name\" [1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetIntegerValues(1u, 1u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too many values in parameter list: name");
}

TEST(Parameter, Integer) {
  std::stringstream input("\"integer name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::INTEGER,
                     spectrum_manager, texture_manager);

  ASSERT_EQ(1u, parameter.GetIntegerValues(1u, 1u).size());
  EXPECT_EQ(1, parameter.GetIntegerValues(1u, 1u).at(0));
}

TEST(Parameter, NormalWrongType) {
  std::stringstream input("\"float name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  EXPECT_EXIT(
      parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::NORMAL,
                         spectrum_manager, texture_manager),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Wrong type for parameter list: name");
}

TEST(Parameter, NormalTooFew) {
  std::stringstream input("\"normal name\" [1 1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::NORMAL,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetNormalValues(0u, 3u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few values in parameter list: name");
}

TEST(Parameter, NormalTooMany) {
  std::stringstream input("\"normal name\" [1 1 1 1 1 1 1 1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::NORMAL,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetNormalValues(1u, 1u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too many values in parameter list: name");
}

TEST(Parameter, Normal) {
  std::stringstream input("\"normal name\" [1 1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::NORMAL,
                     spectrum_manager, texture_manager);

  ASSERT_EQ(1u, parameter.GetNormalValues(1u, 1u).size());
  EXPECT_EQ(iris::Vector(1.0, 1.0, 1.0),
            parameter.GetNormalValues(1u, 1u).at(0));
}

TEST(Parameter, Point3WrongType) {
  std::stringstream input("\"float name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  EXPECT_EXIT(
      parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::POINT3,
                         spectrum_manager, texture_manager),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Wrong type for parameter list: name");
}

TEST(Parameter, Point3TooFew) {
  std::stringstream input("\"point3 name\" [1 1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::POINT3,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetPoint3Values(0u, 3u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few values in parameter list: name");
}

TEST(Parameter, Point3TooMany) {
  std::stringstream input("\"point3 name\" [1 1 1 1 1 1 1 1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::POINT3,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetPoint3Values(1u, 1u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too many values in parameter list: name");
}

TEST(Parameter, Point3) {
  std::stringstream input("\"point3 name\" [1 1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::POINT3,
                     spectrum_manager, texture_manager);

  ASSERT_EQ(1u, parameter.GetPoint3Values(1u, 1u).size());
  EXPECT_EQ(iris::Point(1.0, 1.0, 1.0),
            parameter.GetPoint3Values(1u, 1u).at(0));
}

// REFLECTOR_TEXTURE
// SPECTRUM
// STRING

TEST(Parameter, Vector3WrongType) {
  std::stringstream input("\"float name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  EXPECT_EXIT(parameter.LoadFrom(parameter_list,
                                 iris::pbrt_frontend::Parameter::VECTOR3,
                                 spectrum_manager, texture_manager),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Wrong type for parameter list: name");
}

TEST(Parameter, Vector3TooFew) {
  std::stringstream input("\"vector3 name\" [1 1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::VECTOR3,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetVector3Values(0u, 3u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few values in parameter list: name");
}

TEST(Parameter, Vector3TooMany) {
  std::stringstream input("\"vector3 name\" [1 1 1 1 1 1 1 1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::VECTOR3,
                     spectrum_manager, texture_manager);

  EXPECT_EXIT(parameter.GetVector3Values(1u, 1u),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too many values in parameter list: name");
}

TEST(Parameter, Vector3) {
  std::stringstream input("\"vector3 name\" [1 1 1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  ASSERT_TRUE(parameter_list.ParseFrom(tokenizer, iris::Color::LINEAR_SRGB));

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  iris::pbrt_frontend::Parameter parameter;
  parameter.LoadFrom(parameter_list, iris::pbrt_frontend::Parameter::VECTOR3,
                     spectrum_manager, texture_manager);

  ASSERT_EQ(1u, parameter.GetVector3Values(1u, 1u).size());
  EXPECT_EQ(iris::Vector(1.0, 1.0, 1.0),
            parameter.GetVector3Values(1u, 1u).at(0));
}