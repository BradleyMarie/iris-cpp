#ifndef _IRIS_MATERIAL_
#define _IRIS_MATERIAL_

#include "iris/bxdf.h"
#include "iris/bxdf_allocator.h"
#include "iris/texture_coordinates.h"

namespace iris {

class Material {
 public:
  virtual const Bxdf* Compute(const TextureCoordinates& texture_coordinates,
                              BxdfAllocator& allocator) const = 0;
  virtual ~Material() {}
};

}  // namespace iris

#endif  // _IRIS_MATERIAL_