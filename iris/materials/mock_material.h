#ifndef _IRIS_MATERIALS_MOCK_MATERIAL_
#define _IRIS_MATERIALS_MOCK_MATERIAL_

#include "googlemock/include/gmock/gmock.h"
#include "iris/bxdf.h"
#include "iris/bxdf_allocator.h"
#include "iris/material.h"
#include "iris/spectral_allocator.h"
#include "iris/texture_coordinates.h"

namespace iris {
namespace materials {

class MockMaterial final : public Material {
 public:
  MOCK_METHOD(const Bxdf*, Evaluate,
              (const TextureCoordinates&, SpectralAllocator&, BxdfAllocator&),
              (const override));
};

}  // namespace materials
}  // namespace iris

#endif  // _IRIS_MATERIALS_MOCK_MATERIAL_