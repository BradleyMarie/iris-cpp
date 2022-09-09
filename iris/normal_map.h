#ifndef _IRIS_NORMAL_MAP_
#define _IRIS_NORMAL_MAP_

#include "iris/float.h"
#include "iris/texture_coordinates.h"
#include "iris/vector.h"

namespace iris {

class NormalMap {
 public:
  virtual Vector Compute(const TextureCoordinates& texture_coordinates,
                         const Vector& surface_normal) const = 0;
  virtual ~NormalMap() {}
};

}  // namespace iris

#endif  // _IRIS_NORMAL_MAP_