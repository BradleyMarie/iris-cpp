#ifndef _IRIS_EMISSIVE_MOCK_EMISSIVE_MATERIAL_
#define _IRIS_EMISSIVE_MOCK_EMISSIVE_MATERIAL_

#include "googlemock/include/gmock/gmock.h"
#include "iris/spectrum.h"
#include "iris/texture_coordinates.h"

namespace iris {
namespace emissive {

class MockEmissiveSpectrum : public Spectrum {
 public:
  MOCK_METHOD(const Spectrum*, Compute, (const TextureCoordinates&),
              (const override));
};

}  // namespace emissive
}  // namespace iris

#endif  // _IRIS_EMISSIVE_MOCK_EMISSIVE_MATERIAL_