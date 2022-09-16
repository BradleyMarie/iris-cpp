#ifndef _IRIS_BSDFS_LAMBERTIAN_BRDF_
#define _IRIS_BSDFS_LAMBERTIAN_BRDF_

#include <optional>

#include "iris/bsdf.h"
#include "iris/float.h"
#include "iris/random.h"
#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {
namespace bsdfs {

class LambertianBrdf final : public Bsdf {
 public:
  LambertianBrdf(const Reflector& reflector) : reflector_(reflector) {}

  std::optional<Sample> SampleAll(const Vector& incoming,
                                  const Vector& surface_normal,
                                  const Vector& shading_normal, Random& rng,
                                  SpectralAllocator& allocator) const override;

  std::optional<Sample> SampleDiffuse(
      const Vector& incoming, const Vector& surface_normal,
      const Vector& shading_normal, Random& rng,
      SpectralAllocator& allocator) const override;

  const Reflector* ReflectanceAll(const Vector& incoming,
                                  const Vector& surface_normal,
                                  const Vector& shading_normal,
                                  const Vector& outgoing, bool transmitted,
                                  SpectralAllocator& allocator,
                                  visual_t* pdf = nullptr) const override;

  const Reflector* ReflectanceDiffuse(const Vector& incoming,
                                      const Vector& surface_normal,
                                      const Vector& shading_normal,
                                      const Vector& outgoing, bool transmitted,
                                      SpectralAllocator& allocator,
                                      visual_t* pdf = nullptr) const override;

 private:
  const Reflector& reflector_;
};

}  // namespace bsdfs
}  // namespace iris

#endif  // _IRIS_BSDFS_LAMBERTIAN_BRDF_