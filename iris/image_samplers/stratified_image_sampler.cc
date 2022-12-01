#include "iris/image_samplers/stratified_image_sampler.h"

namespace iris {
namespace image_samplers {
namespace {

geometric_t ToImageCoordinates(size_t pixel_index, size_t image_size,
                               uint16_t subpixel_index,
                               geometric_t subpixel_size, geometric value) {
  subpixel_size /= static_cast<geometric_t>(image_size);
  geometric_t base = static_cast<geometric_t>(pixel_index) /
                     static_cast<geometric_t>(image_size);
  return base + static_cast<geometric_t>(subpixel_index) * subpixel_size +
         value * subpixel_size;
}

}  // namespace

ImageSampler::Sample StratifiedImageSampler::SamplePixel(
    std::pair<size_t, size_t> image_dimensions, std::pair<size_t, size_t> pixel,
    uint32_t sample_index, bool sample_lens, Random& rng) {
  uint16_t x_index = sample_index % x_samples_;
  uint16_t y_index = sample_index / x_samples_;

  geometric x_sample = jittered_ ? rng.NextGeometric() : 0.5;
  geometric y_sample = jittered_ ? rng.NextGeometric() : 0.5;

  geometric_t image_u =
      ToImageCoordinates(pixel.second, image_dimensions.second, x_index,
                         subpixel_size_x_, x_sample);
  geometric_t image_v = ToImageCoordinates(pixel.first, image_dimensions.first,
                                           y_index, subpixel_size_y_, y_sample);

  if (!sample_lens) {
    return {{image_u, image_v}, {}, rng};
  }

  return {
      {image_u, image_v}, {{rng.NextGeometric(), rng.NextGeometric()}}, rng};
}

uint32_t StratifiedImageSampler::SamplesPerPixel() const {
  return x_samples_ * y_samples_;
}

std::unique_ptr<ImageSampler> StratifiedImageSampler::Duplicate() const {
  return std::make_unique<StratifiedImageSampler>(x_samples_, y_samples_,
                                                  jittered_);
}

}  // namespace image_samplers
}  // namespace iris