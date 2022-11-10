#ifndef _IRIS_ENVIRONMENTAL_LIGHT_
#define _IRIS_ENVIRONMENTAL_LIGHT_

#include <optional>

#include "iris/float.h"
#include "iris/reference_countable.h"
#include "iris/sampler.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/vector.h"

namespace iris {

class EnvironmentalLight : public ReferenceCountable {
 public:
  struct SampleResult {
    const Spectrum& emission;
    const Vector to_light;
    const visual_t pdf;
  };

  virtual std::optional<SampleResult> Sample(
      Sampler& sampler, SpectralAllocator& allocator) const = 0;

  virtual const Spectrum* Emission(const Vector& to_light,
                                   SpectralAllocator& allocator,
                                   visual_t* pdf = nullptr) const = 0;

  virtual ~EnvironmentalLight() {}
};

}  // namespace iris

#endif  // _IRIS_LIGHT_