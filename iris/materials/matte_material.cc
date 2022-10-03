#include "iris/materials/matte_material.h"

#include "iris/bxdfs/lambertian_brdf.h"

namespace iris {
namespace materials {

const Bxdf* MatteMaterial::Evaluate(
    const TextureCoordinates& texture_coordinates,
    SpectralAllocator& spectral_allocator,
    BxdfAllocator& bxdf_allocator) const {
  auto* reflector =
      reflectance_->Evaluate(texture_coordinates, spectral_allocator);
  if (reflector == nullptr) {
    return nullptr;
  }

  return &bxdf_allocator.Allocate<bxdfs::LambertianBrdf>(*reflector);
}

}  // namespace materials
}  // namespace iris