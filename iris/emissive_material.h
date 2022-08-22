#ifndef _IRIS_EMISSIVE_MATERIAL_
#define _IRIS_EMISSIVE_MATERIAL_

#include "iris/spectrum.h"
#include "iris/texture_coordinates.h"

namespace iris {

class EmissiveMaterial {
 public:
  virtual const Spectrum* Compute(
      const TextureCoordinates& texture_coordinates) const = 0;
  virtual ~EmissiveMaterial() {}
};

}  // namespace iris

#endif  // _IRIS_EMISSIVE_MATERIAL_