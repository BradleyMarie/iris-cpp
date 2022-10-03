#ifndef _IRIS_TESTING_EMISSIVE_MATERIAL_TESTER_
#define _IRIS_TESTING_EMISSIVE_MATERIAL_TESTER_

#include "iris/emissive_material.h"
#include "iris/internal/arena.h"
#include "iris/spectral_allocator.h"
#include "iris/spectrum.h"
#include "iris/texture_coordinates.h"

namespace iris {
namespace testing {

class EmissiveMaterialTester final {
 public:
  const Spectrum* Evaluate(const EmissiveMaterial& material,
                           const TextureCoordinates& texture_coordinates);

 private:
  internal::Arena arena_;
};

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_EMISSIVE_MATERIAL_TESTER_