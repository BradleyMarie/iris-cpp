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
  enum Type {
    BRDF,
    BTDF,
  };

  struct SampleResult {
    const Reflector& reflector;
    Vector direction;
    std::optional<visual_t> pdf;
  };

  virtual SampleResult Sample(const Vector& incoming, Random& rng,
                              SpectralAllocator& allocator) const = 0;

  virtual const Reflector* Reflectance(const Vector& incoming,
                                       const Vector& outgoing, Type type,
                                       SpectralAllocator& allocator,
                                       visual_t* pdf = nullptr) const = 0;

 protected:
  ~Bxdf() = default;
};

}  // namespace iris

#endif  // _IRIS_BXDF_