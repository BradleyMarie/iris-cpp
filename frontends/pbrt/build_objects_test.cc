#include "frontends/pbrt/build_objects.h"

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

class TestSpectrumManager final : public iris::pbrt_frontend::SpectrumManager {
 public:
  iris::ReferenceCounted<iris::Spectrum> AllocateSpectrum(
      const std::map<iris::visual, iris::visual>& wavelengths) override {
    return iris::ReferenceCounted<iris::Spectrum>();
  }

  iris::ReferenceCounted<iris::Spectrum> AllocateSpectrum(
      const iris::pbrt_frontend::Color& color) override {
    return iris::ReferenceCounted<iris::Spectrum>();
  }

  iris::ReferenceCounted<iris::Reflector> AllocateReflector(
      const std::map<iris::visual, iris::visual>& wavelengths) override {
    return iris::ReferenceCounted<iris::Reflector>();
  }

  iris::ReferenceCounted<iris::Reflector> AllocateReflector(
      const iris::pbrt_frontend::Color& color) override {
    return iris::ReferenceCounted<iris::Reflector>();
  }
};

TEST(ObjectBuilder, Success) {
  std::stringstream input("\"integer name\" [123] \"integer name2\" [456]");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  std::unordered_map<std::string_view, iris::pbrt_frontend::Parameter::Type>
      parameters = {{"name", iris::pbrt_frontend::Parameter::INTEGER},
                    {"name2", iris::pbrt_frontend::Parameter::INTEGER}};
  TestObjectBuilder object_builder(parameters);

  TestSpectrumManager spectrum_manager;
  iris::pbrt_frontend::TextureManager texture_manager;
  EXPECT_EQ(1337,
            iris::pbrt_frontend::BuildObject(
                object_builder, tokenizer, spectrum_manager, texture_manager));
}