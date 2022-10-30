#include "iris/light_scenes/all_light_scene.h"

#include <limits>

#include "googletest/include/gtest/gtest.h"
#include "iris/lights/mock_light.h"
#include "iris/random/mock_random.h"
#include "iris/scenes/list_scene.h"
#include "iris/testing/light_sample_allocator.h"

TEST(AllLightSceneTest, NoLights) {
  iris::random::MockRandom rng;
  auto scene_objects = iris::SceneObjects::Builder().Build();
  auto light_scene =
      iris::light_scenes::AllLightScene::Builder::Create()->Build(
          scene_objects);
  EXPECT_EQ(nullptr,
            light_scene->Sample(iris::Point(0.0, 0.0, 0.0), rng,
                                iris::testing::GetLightSampleAllocator()));
}

TEST(AllLightSceneTest, TwoLights) {
  auto light0 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  auto* light0_ptr = light0.Get();

  auto light1 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  auto* light1_ptr = light1.Get();

  iris::random::MockRandom rng;
  auto light_scene_builder = iris::SceneObjects::Builder();
  light_scene_builder.Add(std::move(light0));
  light_scene_builder.Add(std::move(light1));
  auto objects = light_scene_builder.Build();
  auto light_scene =
      iris::light_scenes::AllLightScene::Builder::Create()->Build(objects);

  auto light_samples =
      light_scene->Sample(iris::Point(0.0, 0.0, 0.0), rng,
                          iris::testing::GetLightSampleAllocator());
  ASSERT_NE(nullptr, light_samples);

  auto* light1_sample = light_samples;
  EXPECT_EQ(light1_ptr, &light1_sample->light);
  EXPECT_FALSE(light1_sample->pdf);
  ASSERT_NE(nullptr, light1_sample->next);

  auto* light0_sample = light1_sample->next;
  EXPECT_EQ(light0_ptr, &light0_sample->light);
  EXPECT_FALSE(light0_sample->pdf);
  EXPECT_EQ(nullptr, light0_sample->next);
}