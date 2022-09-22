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
    REFLECTION = 0x1,
    TRANSMISSION = 0x2,
    SPECULAR = 0x4,
    DIFFUSE = 0x8,
    ALL = 0xF,
  };

  Bxdf(Type type) : type(type) {}

  struct Sample {
    const Reflector& reflector;
    Vector direction;
    std::optional<visual_t> pdf;
    Type type;
  };

  virtual Sample Sample(const Vector& incoming, Random& rng,
                        SpectralAllocator& allocator) const = 0;

  virtual const Reflector* Reflectance(const Vector& incoming,
                                       const Vector& outgoing,
                                       SpectralAllocator& allocator,
                                       visual_t* pdf = nullptr) const = 0;

  const Type type;

 protected:
  ~Bxdf() = default;
};

}  // namespace iris

#endif  // _IRIS_BXDF_