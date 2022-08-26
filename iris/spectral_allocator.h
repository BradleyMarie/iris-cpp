#ifndef _IRIS_SPECTRAL_ALLOCATOR_
#define _IRIS_SPECTRAL_ALLOCATOR_

#include "iris/reflector.h"
#include "iris/spectrum.h"

namespace iris {
namespace internal {
class Arena;
}  // namespace internal

class SpectralAllocator {
 public:
  SpectralAllocator(internal::Arena& arena) : arena_(arena) {}

  const Spectrum* Add(const Spectrum* addend0, const Spectrum* addend1);
  const Spectrum* Scale(const Spectrum* spectrum, visual_t attenuation);
  const Spectrum* Reflect(const Spectrum* spectrum, const Reflector* reflector);

  const Reflector* Add(const Reflector* addend0, const Reflector* addend1);
  const Reflector* Scale(const Reflector* reflector, visual_t attenuation);

 private:
  internal::Arena& arena_;
};

}  // namespace iris

#endif  // _IRIS_HIT_ALLOCATOR_