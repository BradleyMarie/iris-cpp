#include "iris/image_samplers/random_image_sampler.h"

namespace iris {
namespace image_samplers {
namespace {

geometric_t ToImageCoordinates(size_t index, size_t size, geometric value) {
  geometric_t base =
      static_cast<geometric_t>(index) / static_cast<geometric_t>(size);
  return base + (value / static_cast<geometric_t>(size));
}

}  // namespace

ImageSampler::Sample RandomImageSampler::SamplePixel(
    std::pair<size_t, size_t> image_dimensions, std::pair<size_t, size_t> pixel,
    uint32_t sample_index, bool sample_lens, Random& rng) {
  geometric_t image_u = ToImageCoordinates(
      pixel.second, image_dimensions.second, rng.NextGeometric());
  geometric_t image_v = ToImageCoordinates(pixel.first, image_dimensions.first,
                                           rng.NextGeometric());

  if (!sample_lens) {
    return {{image_u, image_v}, {}, rng};
  }

  return {
      {image_u, image_v}, {{rng.NextGeometric(), rng.NextGeometric()}}, rng};
}

uint32_t RandomImageSampler::SamplesPerPixel() const {
  return samples_per_pixel_;
}

std::unique_ptr<ImageSampler> RandomImageSampler::Duplicate() const {
  return std::make_unique<RandomImageSampler>(samples_per_pixel_);
}

}  // namespace image_samplers
}  // namespace iris