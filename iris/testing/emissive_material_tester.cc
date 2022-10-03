#include "iris/testing/emissive_material_tester.h"

namespace iris {
namespace testing {

const Spectrum* EmissiveMaterialTester::Evaluate(
    const EmissiveMaterial& material,
    const TextureCoordinates& texture_coordinates) {
  SpectralAllocator spectral_allocator(arena_);
  return material.Evaluate(texture_coordinates, spectral_allocator);
}

}  // namespace testing
}  // namespace iris