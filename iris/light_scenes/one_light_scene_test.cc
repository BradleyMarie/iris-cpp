#include "iris/light_scenes/one_light_scene.h"

#include <limits>

#include "googletest/include/gtest/gtest.h"
#include "iris/lights/mock_light.h"
#include "iris/random/mock_random.h"
#include "iris/scenes/list_scene.h"
#include "iris/testing/light_sample_allocator.h"

TEST(OneLightSceneTest, NoLights) {
  iris::random::MockRandom rng;
  auto scene_objects = iris::SceneObjects::Builder().Build();
  auto light_scene =
      iris::light_scenes::OneLightScene::Builder::Create()->Build(
          scene_objects);
  EXPECT_EQ(nullptr,
            light_scene->Sample(iris::Point(0.0, 0.0, 0.0), rng,
                                iris::testing::GetLightSampleAllocator()));
}

TEST(OneLightSceneTest, OneLight) {
  auto light0 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  auto* light0_ptr = light0.Get();

  iris::random::MockRandom rng;
  auto light_scene_builder = iris::SceneObjects::Builder();
  light_scene_builder.Add(std::move(light0));
  auto objects = light_scene_builder.Build();
  auto light_scene =
      iris::light_scenes::OneLightScene::Builder::Create()->Build(objects);

  auto light_sample =
      light_scene->Sample(iris::Point(0.0, 0.0, 0.0), rng,
                          iris::testing::GetLightSampleAllocator());
  ASSERT_NE(nullptr, light_sample);

  EXPECT_EQ(light0_ptr, &light_sample->light);
  EXPECT_FALSE(light_sample->pdf);
  ASSERT_EQ(nullptr, light_sample->next);
}

TEST(OneLightSceneTest, TwoLights) {
  auto light0 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  auto light1 = iris::MakeReferenceCounted<iris::lights::MockLight>();
  auto* light1_ptr = light1.Get();

  iris::random::MockRandom rng;
  auto light_scene_builder = iris::SceneObjects::Builder();
  light_scene_builder.Add(std::move(light0));
  light_scene_builder.Add(std::move(light1));
  auto objects = light_scene_builder.Build();
  auto light_scene =
      iris::light_scenes::OneLightScene::Builder::Create()->Build(objects);

  EXPECT_CALL(rng, NextIndex(2)).WillOnce(testing::Return(1));

  auto light_sample =
      light_scene->Sample(iris::Point(0.0, 0.0, 0.0), rng,
                          iris::testing::GetLightSampleAllocator());
  ASSERT_NE(nullptr, light_sample);

  EXPECT_EQ(light1_ptr, &light_sample->light);
  EXPECT_EQ(0.5, light_sample->pdf);
  ASSERT_EQ(nullptr, light_sample->next);
}