#ifndef _IRIS_INTEGRATORS_INTERNAL_PATH_BUILDER_
#define _IRIS_INTEGRATORS_INTERNAL_PATH_BUILDER_

#include <vector>

#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"

namespace iris {
namespace integrators {
namespace internal {

class PathBuilder {
 public:
  PathBuilder(std::vector<const Reflector*>& reflectors,
              std::vector<const Spectrum*>& spectra,
              std::vector<visual_t>& attenuations) noexcept;

  void Add(const Spectrum* spectrum, SpectralAllocator& allocator);
  void Bounce(const Reflector* reflector, visual_t attenuation);

  const Spectrum* Build(SpectralAllocator& allocator) const;

 private:
  std::vector<const Reflector*>& reflectors_;
  std::vector<const Spectrum*>& spectra_;
  std::vector<visual_t>& attenuations_;
};

}  // namespace internal
}  // namespace integrators
}  // namespace iris

#endif  // _IRIS_INTEGRATORS_INTERNAL_PATH_BUILDER_