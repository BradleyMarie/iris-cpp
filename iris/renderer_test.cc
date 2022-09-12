#include "iris/renderer.h"

#include <thread>

#include "googletest/include/gtest/gtest.h"
#include "iris/cameras/mock_camera.h"
#include "iris/color_matchers/mock_color_matcher.h"
#include "iris/image_samplers/mock_image_sampler.h"
#include "iris/integrators/mock_integrator.h"
#include "iris/light_scenes/all_light_scene.h"
#include "iris/random/mock_random.h"
#include "iris/scenes/list_scene.h"

const iris::Spectrum* spectrum = reinterpret_cast<const iris::Spectrum*>(3);

void RunTestBody(unsigned num_threads_requested, unsigned actual_num_threads) {
  auto scene_builder = iris::scenes::ListScene::Builder::Create();
  auto light_scene_builder =
      iris::light_scenes::AllLightScene::Builder::Create();
  iris::Renderer renderer(*scene_builder, *light_scene_builder);

  iris::Ray trace_ray(iris::Point(0.0, 0.0, 0.0), iris::Vector(0.0, 0.0, 1.0));
  std::pair<size_t, size_t> image_dimensions = std::make_pair(32, 33);
  uint32_t samples_per_pixel = 2;
  size_t pixels = image_dimensions.first * image_dimensions.second;
  size_t samples = pixels * samples_per_pixel;
  size_t chunks = 64;

  iris::random::MockRandom rng;
  EXPECT_CALL(rng, Replicate())
      .Times(chunks)
      .WillRepeatedly(testing::Invoke(
          []() { return std::make_unique<iris::random::MockRandom>(); }));

  size_t sampler_index = 0;
  iris::image_samplers::MockImageSampler image_sampler;
  EXPECT_CALL(image_sampler, Duplicate())
      .Times(chunks)
      .WillRepeatedly(testing::Invoke([&]() {
        auto result =
            std::make_unique<iris::image_samplers::MockImageSampler>();
        EXPECT_CALL(*result, SamplesPerPixel())
            .WillOnce(testing::Return(samples_per_pixel));
        EXPECT_CALL(*result, SamplePixel(testing::_, testing::_, testing::_,
                                         testing::_, testing::_))
            .Times(sampler_index++ % 2 == 0 ? samples_per_pixel * 32
                                            : samples_per_pixel)
            .WillRepeatedly(testing::Return(
                iris::ImageSampler::Sample{{0.0, 0.0}, std::nullopt, rng}));
        return result;
      }));

  iris::cameras::MockCamera camera;
  EXPECT_CALL(camera, HasLens())
      .Times(actual_num_threads)
      .WillRepeatedly(testing::Return(false));

  EXPECT_CALL(camera, Compute(testing::_, testing::_))
      .Times(samples)
      .WillRepeatedly(testing::Return(trace_ray));

  iris::integrators::MockIntegrator integrator;
  EXPECT_CALL(integrator, Integrate(trace_ray, testing::_, testing::_,
                                    testing::_, testing::_, testing::_))
      .Times(samples)
      .WillRepeatedly(testing::Return(spectrum));

  iris::Color color(1.0, 1.0, 1.0, iris::Color::LINEAR_SRGB);
  iris::color_matchers::MockColorMatcher color_matcher;
  EXPECT_CALL(color_matcher, Match(testing::_))
      .Times(samples)
      .WillRepeatedly(
          testing::Return(std::array<iris::visual_t, 3>({1.0, 1.0, 1.0})));
  EXPECT_CALL(color_matcher, ColorSpace())
      .Times(actual_num_threads)
      .WillRepeatedly(testing::Return(iris::Color::LINEAR_SRGB));

  auto framebuffer =
      renderer.Render(camera, image_sampler, integrator, color_matcher, rng,
                      image_dimensions, 0.0, num_threads_requested);
  EXPECT_EQ(image_dimensions, framebuffer.Size());

  for (size_t y = 0; y < image_dimensions.first; y++) {
    for (size_t x = 0; x < image_dimensions.second; x++) {
      EXPECT_EQ(color, framebuffer.Get(y, x));
    }
  }
}

TEST(RendererTest, SingleThreaded) { RunTestBody(1u, 1u); }

TEST(RendererTest, MultiThreaded) {
  RunTestBody(0u, std::thread::hardware_concurrency());
}