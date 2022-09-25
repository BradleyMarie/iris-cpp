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

  virtual Vector Sample(const Vector& incoming, Random& rng) const = 0;

  virtual visual_t DiffusePdf(const Vector& incoming,
                              const Vector& outgoing) const = 0;

  virtual std::optional<visual_t> SamplePdf(const Vector& incoming,
                                            const Vector& outgoing) const = 0;

  virtual const Reflector* Reflectance(const Vector& incoming,
                                       const Vector& outgoing, Type type,
                                       SpectralAllocator& allocator) const = 0;

 protected:
  ~Bxdf() = default;
};

}  // namespace iris

#endif  // _IRIS_BXDF_