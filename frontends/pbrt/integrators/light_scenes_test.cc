#include "frontends/pbrt/integrators/light_scenes.h"

#include "googletest/include/gtest/gtest.h"

TEST(ParseLightScene, InvalidType) {
  EXPECT_EXIT(
      iris::pbrt_frontend::integrators::ParseLightScene("NotAThing"),
      testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Unsupported value for parameter lightsamplestrategy: NotAThing");
}

TEST(ParseLightScene, All) {
  auto light_scene = iris::pbrt_frontend::integrators::ParseLightScene("all");
  EXPECT_NE(nullptr, light_scene.get());
}

TEST(ParseLightScene, Uniform) {
  auto light_scene =
      iris::pbrt_frontend::integrators::ParseLightScene("uniform");
  EXPECT_NE(nullptr, light_scene.get());
}