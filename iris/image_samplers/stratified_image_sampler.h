#ifndef _IRIS_IMAGE_SAMPLERS_STRATIFIED_IMAGE_SAMPLER_
#define _IRIS_IMAGE_SAMPLERS_STRATIFIED_IMAGE_SAMPLER_

#include <array>
#include <memory>
#include <optional>
#include <utility>

#include "iris/float.h"
#include "iris/image_sampler.h"
#include "iris/random.h"

namespace iris {
namespace image_samplers {

class StratifiedImageSampler final : public ImageSampler {
 public:
  StratifiedImageSampler(uint16_t x_samples, uint16_t y_samples,
                         bool jittered) noexcept;

  Sample SamplePixel(std::pair<size_t, size_t> image_dimensions,
                     std::pair<size_t, size_t> pixel, uint32_t sample_index,
                     bool sample_lens, Random& rng) override;
  uint32_t SamplesPerPixel() const override;

  std::unique_ptr<ImageSampler> Duplicate() const override;

 private:
  const uint16_t x_samples_;
  const uint16_t y_samples_;
  const geometric_t subpixel_size_x_;
  const geometric_t subpixel_size_y_;
  const bool jittered_;
};

}  // namespace image_samplers
}  // namespace iris

#endif  // _IRIS_IMAGE_SAMPLERS_STRATIFIED_IMAGE_SAMPLER_