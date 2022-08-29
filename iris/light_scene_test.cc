#include "iris/light_scene.h"

#include <optional>
#include <vector>

#include "googletest/include/gtest/gtest.h"
#include "iris/scenes/list_scene.h"

class TestLightSceneBuilder final : public iris::LightScene::Builder {
 public:
  TestLightSceneBuilder(
      size_t num_expected_lights,
      std::optional<std::vector<iris::Light*>> expected_lights = std::nullopt)
      : num_expected_lights_(num_expected_lights),
        expected_lights_(expected_lights) {}

  std::unique_ptr<iris::LightScene> Build(
      std::vector<std::unique_ptr<iris::Light>> lights) override;

 private:
  size_t num_expected_lights_;
  std::optional<std::vector<iris::Light*>> expected_lights_;
};

std::unique_ptr<iris::LightScene> TestLightSceneBuilder::Build(
    std::vector<std::unique_ptr<iris::Light>> lights) {
  EXPECT_EQ(num_expected_lights_, lights.size());
  if (expected_lights_) {
    EXPECT_EQ(expected_lights_->size(), lights.size());
    for (size_t i = 0u; i < expected_lights_->size(); i++) {
      EXPECT_EQ(expected_lights_->at(i), lights.at(i).get());
    }
  }

  return nullptr;
}

class TestLight final : public iris::Light {
 public:
  std::optional<SampleResult> Sample(
      const iris::Point& hit_point, iris::Random& rng,
      iris::VisibilityTester& tester,
      iris::SpectralAllocator& allocator) const override {
    EXPECT_TRUE(false);
    return std::nullopt;
  }

  const iris::Spectrum* Emission(const iris::Ray& to_light,
                                 iris::VisibilityTester& tester,
                                 iris::SpectralAllocator& allocator,
                                 iris::visual_t* pdf) const override {
    EXPECT_TRUE(false);
    return nullptr;
  }
};

TEST(ListSceneTest, BuilderEmpty) {
  auto scene = iris::scenes::ListScene::Builder::Create()->Build();
  std::unique_ptr<iris::LightScene::Builder> builder =
      std::make_unique<TestLightSceneBuilder>(0);
  ASSERT_EQ(nullptr, builder->Build(*scene).get());
}

TEST(ListSceneTest, BuilderOneLight) {
  auto test_light = std::make_unique<TestLight>();
  auto scene = iris::scenes::ListScene::Builder::Create()->Build();
  std::unique_ptr<iris::LightScene::Builder> builder =
      std::make_unique<TestLightSceneBuilder>(
          1, std::vector<iris::Light*>({test_light.get()}));
  builder->Add(std::move(test_light));
  ASSERT_EQ(nullptr, builder->Build(*scene).get());
}

// TODO: Test AreaLight Creation
// TODO: Test AreaLight Logic