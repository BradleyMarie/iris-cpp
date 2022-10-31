#ifndef _IRIS_BSDF_
#define _IRIS_BSDF_

#include <optional>
#include <utility>

#include "iris/bxdf.h"
#include "iris/float.h"
#include "iris/reflector.h"
#include "iris/sampler.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {

class Bsdf final {
 public:
  Bsdf(const Bxdf& bxdf, const Vector& surface_normal,
       const Vector& shading_normal, bool normalize = false) noexcept;

  struct SampleResult {
    const Reflector& reflector;
    const Vector direction;
    const std::optional<visual_t> pdf;
  };

  std::optional<SampleResult> Sample(const Vector& incoming, Sampler sampler,
                                     SpectralAllocator& allocator) const;

  struct ReflectanceResult {
    const Reflector& reflector;
    const visual_t pdf;
  };

  std::optional<ReflectanceResult> Reflectance(
      const Vector& incoming, const Vector& outgoing,
      SpectralAllocator& allocator) const;

 private:
  Bsdf(const Bxdf& bxdf, const Vector vectors[4]) noexcept
      : bxdf_(bxdf),
        x_(vectors[0]),
        y_(vectors[1]),
        z_(vectors[2]),
        surface_normal_(vectors[3]) {}

  Vector ToLocal(const Vector& vector) const;
  Vector ToWorld(const Vector& vector) const;

  const Bxdf& bxdf_;
  const Vector x_, y_, z_;
  const Vector surface_normal_;
};

}  // namespace iris

#endif  // _IRIS_BSDF_