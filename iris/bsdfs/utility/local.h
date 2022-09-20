#ifndef _IRIS_BSDFS_UTILITY_LOCAL_
#define _IRIS_BSDFS_UTILITY_LOCAL_

#include <utility>

#include "iris/vector.h"

namespace iris {
namespace bsdfs {
namespace utility {

std::pair<Vector, Vector> CreateLocalCoordinateSpace(
    const Vector& surface_normal) {
  geometric values[] = {0.0, 0.0, 0.0};
  values[surface_normal.DiminishedAxis()] = 1.0;
  Vector orthogonal0 =
      CrossProduct(surface_normal, Vector(values[0], values[1], values[2]));
  Vector orthogonal1 = CrossProduct(surface_normal, orthogonal0);
  return std::make_pair(orthogonal0, orthogonal1);
}

Vector Transform(const Vector& vector, const Vector& up,
                 const Vector& orthogonal0, const Vector& orthogonal1) {
  return Vector(
      orthogonal0.x * vector.x + orthogonal1.x * vector.y + up.x * vector.z,
      orthogonal0.y * vector.x + orthogonal1.y * vector.y + up.y * vector.z,
      orthogonal0.z * vector.x + orthogonal1.z * vector.y + up.z * vector.z);
}

}  // namespace utility
}  // namespace bsdfs
}  // namespace iris

#endif  // _IRIS_BSDFS_UTILITY_LOCAL_