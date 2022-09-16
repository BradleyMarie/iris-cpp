#ifndef _IRIS_BSDF_
#define _IRIS_BSDF_

#include <optional>

#include "iris/float.h"
#include "iris/random.h"
#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {

class Bsdf {
 public:
  struct Sample {
    const Reflector& reflector;
    Vector direction;
    bool transmitted;
    bool specular;
    std::optional<visual_t> pdf;
  };

  virtual std::optional<Sample> SampleAll(
      const Vector& incoming, const Vector& surface_normal,
      const Vector& shading_normal, Random& rng,
      SpectralAllocator& allocator) const = 0;

  virtual std::optional<Sample> SampleDiffuse(
      const Vector& incoming, const Vector& surface_normal,
      const Vector& shading_normal, Random& rng,
      SpectralAllocator& allocator) const = 0;

  virtual const Reflector* ReflectanceAll(
      const Vector& incoming, const Vector& surface_normal,
      const Vector& shading_normal, const Vector& outgoing, bool transmitted,
      SpectralAllocator& allocator, visual_t* pdf = nullptr) const = 0;

  virtual const Reflector* ReflectanceDiffuse(
      const Vector& incoming, const Vector& surface_normal,
      const Vector& shading_normal, const Vector& outgoing, bool transmitted,
      SpectralAllocator& allocator, visual_t* pdf = nullptr) const = 0;
};

}  // namespace iris

#endif  // _IRIS_BSDF_