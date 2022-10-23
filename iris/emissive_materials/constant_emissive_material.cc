#include "iris/emissive_materials/constant_emissive_material.h"

namespace iris {
namespace emissive_materials {

const Spectrum* ConstantEmissiveMaterial::Evaluate(
    const TextureCoordinates& texture_coordinates,
    SpectralAllocator& spectral_allocator) const {
  return spectrum_.Get();
}

}  // namespace emissive_materials
}  // namespace iris