#ifndef _IRIS_TESTING_MATERIAL_TESTER_
#define _IRIS_TESTING_MATERIAL_TESTER_

#include "iris/bxdf.h"
#include "iris/internal/arena.h"
#include "iris/material.h"
#include "iris/texture_coordinates.h"

namespace iris {
namespace testing {

class MaterialTester final {
 public:
  const Bxdf* Evaluate(const Material& material,
                       const TextureCoordinates& texture_coordinates);

 private:
  internal::Arena arena_;
};

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_MATERIAL_TESTER_