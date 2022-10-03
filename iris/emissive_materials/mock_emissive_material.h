#ifndef _IRIS_EMISSIVE_MATERIALS_MOCK_EMISSIVE_MATERIAL_
#define _IRIS_EMISSIVE_MATERIALS_MOCK_EMISSIVE_MATERIAL_

#include "googlemock/include/gmock/gmock.h"
#include "iris/emissive_material.h"
#include "iris/spectral_allocator.h"
#include "iris/texture_coordinates.h"

namespace iris {
namespace emissive_materials {

class MockEmissiveMaterial final : public EmissiveMaterial {
 public:
  MOCK_METHOD(const Spectrum*, Evaluate,
              (const TextureCoordinates&, SpectralAllocator&),
              (const override));
};

}  // namespace emissive_materials
}  // namespace iris

#endif  // _IRIS_EMISSIVE_MATERIALS_MOCK_EMISSIVE_MATERIAL_