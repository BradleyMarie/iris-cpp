#ifndef _IRIS_EMISSIVE_MOCK_EMISSIVE_MATERIAL_
#define _IRIS_EMISSIVE_MOCK_EMISSIVE_MATERIAL_

#include "googlemock/include/gmock/gmock.h"
#include "iris/emissive_material.h"
#include "iris/texture_coordinates.h"

namespace iris {
namespace emissive {

class MockEmissiveMaterial : public EmissiveMaterial {
 public:
  MOCK_METHOD(const Spectrum*, Compute, (const TextureCoordinates&),
              (const override));
};

}  // namespace emissive
}  // namespace iris

#endif  // _IRIS_EMISSIVE_MOCK_EMISSIVE_MATERIAL_