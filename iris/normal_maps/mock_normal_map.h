#ifndef _IRIS_NORMAL_MAPS_MOCK_NORMAL_MAP_
#define _IRIS_NORMAL_MAPS_MOCK_NORMAL_MAP_

#include "googlemock/include/gmock/gmock.h"
#include "iris/float.h"
#include "iris/normal_map.h"
#include "iris/reference_countable.h"
#include "iris/texture_coordinates.h"
#include "iris/vector.h"

namespace iris {
namespace normal_maps {

class MockNormalMap final : public NormalMap {
 public:
  MOCK_METHOD(Vector, Evaluate, (const TextureCoordinates&, const Vector&),
              (const override));
};

}  // namespace normal_maps
}  // namespace iris

#endif  // _IRIS_NORMAL_MAPS_MOCK_NORMAL_MAP_