#ifndef _IRIS_IMAGE_SAMPLER_
#define _IRIS_IMAGE_SAMPLER_

#include <array>
#include <memory>
#include <optional>
#include <utility>

#include "iris/float.h"
#include "iris/random.h"

namespace iris {

class ImageSampler {
 public:
  struct ImageSample {
    std::array<visual_t, 2> image_uv;
    std::optional<std::array<visual_t, 2>> lens_uv;
    Random& rng;
  };

  virtual ImageSample SamplePixel(std::pair<size_t, size_t> image_dimensions,
                                  std::pair<size_t, size_t> pixel,
                                  uint32_t sample_index, bool sample_lens,
                                  Random& rng) = 0;
  virtual uint32_t SamplesPerPixel() const = 0;

  virtual std::unique_ptr<ImageSampler> Duplicate() const = 0;

  virtual ~ImageSampler() {}
};

}  // namespace iris

#endif  // _IRIS_IMAGE_SAMPLER_