#include "iris/light_scenes/all_light_scene.h"

#include <limits>

#include "googletest/include/gtest/gtest.h"
#include "iris/lights/mock_light.h"
#include "iris/random/mock_random.h"
#include "iris/scenes/list_scene.h"
#include "iris/testing/light_scene_tester.h"

TEST(AllLightSceneTest, NoLights) {
  iris::random::MockRandom rng;
  auto scene = iris::scenes::ListScene::Builder::Create()->Build();
  auto light_scene =
      iris::light_scenes::AllLightScene::Builder::Create()->Build(*scene);
  iris::testing::LightSceneTester tester;
  EXPECT_EQ(nullptr,
            tester.Sample(*light_scene, iris::Point(0.0, 0.0, 0.0), rng));
}

TEST(AllLightSceneTest, TwoLights) {
  auto light0 = std::make_unique<iris::lights::MockLight>();
  auto* light0_ptr = light0.get();

  auto light1 = std::make_unique<iris::lights::MockLight>();
  auto* light1_ptr = light1.get();

  iris::random::MockRandom rng;
  auto scene = iris::scenes::ListScene::Builder::Create()->Build();
  auto light_scene_builder =
      iris::light_scenes::AllLightScene::Builder::Create();
  light_scene_builder->Add(std::move(light0));
  light_scene_builder->Add(std::move(light1));
  auto light_scene = light_scene_builder->Build(*scene);

  iris::testing::LightSceneTester tester;
  auto light_samples =
      tester.Sample(*light_scene, iris::Point(0.0, 0.0, 0.0), rng);
  ASSERT_NE(nullptr, light_samples);

  auto* light1_sample = light_samples;
  EXPECT_EQ(light1_ptr, &light1_sample->light);
  EXPECT_EQ(std::numeric_limits<iris::visual_t>::infinity(),
            light1_sample->pdf);
  ASSERT_NE(nullptr, light1_sample->next);

  auto* light0_sample = light1_sample->next;
  EXPECT_EQ(light0_ptr, &light0_sample->light);
  EXPECT_EQ(std::numeric_limits<iris::visual_t>::infinity(),
            light0_sample->pdf);
  EXPECT_EQ(nullptr, light0_sample->next);
}