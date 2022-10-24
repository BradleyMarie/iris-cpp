#ifndef _IRIS_MATERIAL_
#define _IRIS_MATERIAL_

#include "iris/bxdf.h"
#include "iris/bxdf_allocator.h"
#include "iris/reference_countable.h"
#include "iris/spectral_allocator.h"
#include "iris/texture_coordinates.h"

namespace iris {

class Material : public ReferenceCountable {
 public:
  virtual const Bxdf* Evaluate(const TextureCoordinates& texture_coordinates,
                               SpectralAllocator& spectral_allocator,
                               BxdfAllocator& bxdf_allocator) const = 0;
  virtual ~Material() {}
};

}  // namespace iris

#endif  // _IRIS_MATERIAL_