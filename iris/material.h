#ifndef _IRIS_MATERIAL_
#define _IRIS_MATERIAL_

#include "iris/bsdf.h"
#include "iris/texture_coordinates.h"

namespace iris {

class Material {
 public:
  virtual Bsdf* Compute(
      const TextureCoordinates& texture_coordinates) const = 0;
  virtual ~Material() {}
};

}  // namespace iris

#endif  // _IRIS_MATERIAL_