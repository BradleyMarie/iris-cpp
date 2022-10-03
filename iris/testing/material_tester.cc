#include "iris/testing/material_tester.h"

namespace iris {
namespace testing {

const Bxdf* MaterialTester::Evaluate(
    const Material& material, const TextureCoordinates& texture_coordinates) {
  SpectralAllocator spectral_allocator(arena_);
  BxdfAllocator bxdf_allocator(arena_);
  return material.Evaluate(texture_coordinates, spectral_allocator,
                           bxdf_allocator);
}

}  // namespace testing
}  // namespace iris