#ifndef _IRIS_EMISSIVE_MATERIAL_
#define _IRIS_EMISSIVE_MATERIAL_

#include "iris/reference_countable.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/texture_coordinates.h"

namespace iris {

class EmissiveMaterial : public ReferenceCountable {
 public:
  virtual const Spectrum* Evaluate(
      const TextureCoordinates& texture_coordinates,
      SpectralAllocator& spectral_allocator) const = 0;
  virtual ~EmissiveMaterial() {}
};

}  // namespace iris

#endif  // _IRIS_EMISSIVE_MATERIAL_