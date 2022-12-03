#include "frontends/pbrt/object_builder.h"

#include "frontends/pbrt/spectrum_managers/test_spectrum_manager.h"
#include "googletest/include/gtest/gtest.h"

class TestObjectBuilder : public iris::pbrt_frontend::ObjectBuilder<int> {
 public:
  TestObjectBuilder(
      const std::unordered_map<
          std::string_view, iris::pbrt_frontend::Parameter::Type>& parameters)
      : ObjectBuilder(parameters) {}

  int Build(const std::unordered_map<std::string_view,
                                     iris::pbrt_frontend::Parameter>&
                parameters) const override {
    return 0;
  }
};

TEST(ObjectBuilder, SpecifiedTwice) {
  std::stringstream input("\"integer name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_TRUE(parameter_list.ParseFrom(tokenizer));

  std::unordered_map<std::string_view, iris::pbrt_frontend::Parameter::Type>
      parameters = {{"name", iris::pbrt_frontend::Parameter::INTEGER}};
  TestObjectBuilder object_builder(parameters);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  std::unordered_set<std::string_view> already_specified = {"name"};
  EXPECT_EXIT(object_builder.Parse(parameter_list, spectrum_manager,
                                   texture_manager, already_specified),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: A parameter was specified twice: name");
}

TEST(ObjectBuilder, UnknownParameter) {
  std::stringstream input("\"integer name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_TRUE(parameter_list.ParseFrom(tokenizer));

  std::unordered_map<std::string_view, iris::pbrt_frontend::Parameter::Type>
      parameters = {{"aaa", iris::pbrt_frontend::Parameter::INTEGER}};
  TestObjectBuilder object_builder(parameters);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  std::unordered_set<std::string_view> already_specified = {};
  EXPECT_FALSE(object_builder.Parse(parameter_list, spectrum_manager,
                                    texture_manager, already_specified, false));
  EXPECT_EXIT(object_builder.Parse(parameter_list, spectrum_manager,
                                   texture_manager, already_specified),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Unknown parameter: name");
}

TEST(ObjectBuilder, Success) {
  std::stringstream input("\"integer name\" [1]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);
  iris::pbrt_frontend::ParameterList parameter_list;
  EXPECT_TRUE(parameter_list.ParseFrom(tokenizer));

  std::unordered_map<std::string_view, iris::pbrt_frontend::Parameter::Type>
      parameters = {{"name", iris::pbrt_frontend::Parameter::INTEGER}};
  TestObjectBuilder object_builder(parameters);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  std::unordered_set<std::string_view> already_specified = {};
  auto parameter = object_builder.Parse(parameter_list, spectrum_manager,
                                        texture_manager, already_specified);
  ASSERT_TRUE(parameter);
  EXPECT_EQ(iris::pbrt_frontend::Parameter::INTEGER, parameter->GetType());
}