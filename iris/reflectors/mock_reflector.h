#ifndef _IRIS_REFLECTORS_MOCK_REFLECTOR_
#define _IRIS_REFLECTORS_MOCK_REFLECTOR_

#include "googlemock/include/gmock/gmock.h"
#include "iris/float.h"
#include "iris/reflector.h"

namespace iris {
namespace reflectors {

class MockReflector final : public Reflector {
 public:
  MOCK_METHOD(visual_t, Reflectance, (visual_t), (const override));
};

}  // namespace reflectors
}  // namespace iris

#endif  // _IRIS_REFLECTORS_MOCK_REFLECTOR_