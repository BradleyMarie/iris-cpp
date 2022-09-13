#ifndef _IRIS_IMAGE_SAMPLERS_RANDOM_IMAGE_SAMPLER_
#define _IRIS_IMAGE_SAMPLERS_RANDOM_IMAGE_SAMPLER_

#include <array>
#include <memory>
#include <optional>
#include <random>
#include <utility>

#include "iris/float.h"
#include "iris/image_sampler.h"
#include "iris/random.h"

namespace iris {
namespace image_samplers {

class RandomImageSampler final : public ImageSampler {
 public:
  RandomImageSampler(uint32_t samples_per_pixel)
      : distribution_(0.0, 1.0), samples_per_pixel_(samples_per_pixel) {}

  Sample SamplePixel(std::pair<size_t, size_t> image_dimensions,
                     std::pair<size_t, size_t> pixel, uint32_t sample_index,
                     bool sample_lens, Random& rng) override;
  uint32_t SamplesPerPixel() const override;

  std::unique_ptr<ImageSampler> Duplicate() const override;

 private:
  std::uniform_real_distribution<geometric> distribution_;
  uint32_t samples_per_pixel_;
};

}  // namespace image_samplers
}  // namespace iris

#endif  // _IRIS_IMAGE_SAMPLERS_RANDOM_IMAGE_SAMPLER_