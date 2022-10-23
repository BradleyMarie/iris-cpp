#ifndef _IRIS_MATERIALS_MATTE_MATERIAL_
#define _IRIS_MATERIALS_MATTE_MATERIAL_

#include <cassert>
#include <memory>

#include "iris/bxdf.h"
#include "iris/bxdf_allocator.h"
#include "iris/float.h"
#include "iris/material.h"
#include "iris/reference_counted.h"
#include "iris/reflectors/reference_countable_reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/texture_coordinates.h"
#include "iris/textures/texture2d.h"

namespace iris {
namespace materials {

class MatteMaterial final : public Material {
 public:
  MatteMaterial(
      iris::ReferenceCounted<textures::PointerTexture2D<
          iris::reflectors::ReferenceCountableReflector, SpectralAllocator>>
          reflectance,
      iris::ReferenceCounted<textures::ValueTexture2D<visual>> sigma)
      : reflectance_(std::move(reflectance)), sigma_(std::move(sigma)) {
    assert(reflectance_);
    assert(sigma_);
  }

  const Bxdf* Evaluate(const TextureCoordinates& texture_coordinates,
                       SpectralAllocator& spectral_allocator,
                       BxdfAllocator& bxdf_allocator) const override;

 private:
  iris::ReferenceCounted<textures::PointerTexture2D<
      iris::reflectors::ReferenceCountableReflector, SpectralAllocator>>
      reflectance_;
  iris::ReferenceCounted<textures::ValueTexture2D<visual>> sigma_;
};

}  // namespace materials
}  // namespace iris

#endif  // _IRIS_MATERIALS_MATTE_MATERIAL_