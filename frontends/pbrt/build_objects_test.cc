#include "frontends/pbrt/build_objects.h"

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
    EXPECT_TRUE(parameters.contains("name"));
    EXPECT_EQ(123, parameters.at("name").GetIntegerValues().at(0));
    EXPECT_TRUE(parameters.contains("name2"));
    EXPECT_EQ(456, parameters.at("name2").GetIntegerValues().at(0));
    return 1337;
  }
};

TEST(ObjectBuilder, Success) {
  std::stringstream input("\"integer name\" [123] \"integer name2\" [456]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  std::unordered_map<std::string_view, iris::pbrt_frontend::Parameter::Type>
      parameters = {{"name", iris::pbrt_frontend::Parameter::INTEGER},
                    {"name2", iris::pbrt_frontend::Parameter::INTEGER}};
  TestObjectBuilder object_builder(parameters);

  iris::pbrt_frontend::spectrum_managers::TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EQ(1337,
            iris::pbrt_frontend::BuildObject(
                object_builder, tokenizer, spectrum_manager, texture_manager));
}