#ifndef _IRIS_BXDF_
#define _IRIS_BXDF_

#include <optional>

#include "iris/float.h"
#include "iris/random.h"
#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {

class Bxdf {
 public:
  virtual Vector Sample(const Vector& incoming, Random& rng) const = 0;

  enum class SampleSource {
    BXDF,
    LIGHT,
  };

  virtual std::optional<visual_t> Pdf(const Vector& incoming,
                                      const Vector& outgoing,
                                      SampleSource sample_source) const = 0;

  enum class Hemisphere {
    BRDF,
    BTDF,
  };

  virtual const Reflector* Reflectance(const Vector& incoming,
                                       const Vector& outgoing,
                                       SampleSource sample_source,
                                       Hemisphere hemisphere,
                                       SpectralAllocator& allocator) const = 0;

 protected:
  ~Bxdf() = default;
};

}  // namespace iris

#endif  // _IRIS_BXDF_