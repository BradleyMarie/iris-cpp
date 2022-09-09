#include "iris/testing/scene.h"

#include "googletest/include/gtest/gtest.h"
#include "iris/internal/hit.h"

namespace iris {
namespace testing {
namespace {

void TestEmptyScene(Scene::Builder& scene_builder, SceneTester& tester) {
  auto scene = scene_builder.Build();
  tester.Trace(*scene, Ray(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0)),
               -std::numeric_limits<geometric>::infinity(),
               std::numeric_limits<geometric>::infinity(),
               [&](const auto& hits) { EXPECT_EQ(0u, hits.size()); });
}

}  // namespace

void SceneTester::Trace(
    const Scene& scene, const Ray& ray, geometric min_distance,
    geometric max_distance,
    std::function<void(const std::vector<Hit>& hits)> validator) {
  iris::Hit* closest_hit;
  Intersector intersector(ray, min_distance, max_distance, hit_arena_,
                          closest_hit);

  scene.Trace(ray, intersector);

  std::vector<Hit> hits;
  for (; closest_hit; closest_hit = closest_hit->next) {
    const auto* full_hit = static_cast<internal::Hit*>(closest_hit);
    hits.emplace_back(
        Hit{*full_hit->geometry, full_hit->model_to_world, full_hit->distance});
  }

  validator(hits);
}

void SceneTestSuite(Scene::Builder& scene_builder) {
  SceneTester tester;
  scene_builder.Build().reset();

  TestEmptyScene(scene_builder, tester);
  // TODO: Add More Test Cases
}

}  // namespace testing
}  // namespace iris