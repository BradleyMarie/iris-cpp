#ifndef _IRIS_RAY_
#define _IRIS_RAY_

#include <cassert>
#include <cmath>

#include "iris/float.h"
#include "iris/point.h"
#include "iris/vector.h"

namespace iris {

struct Ray final {
  explicit Ray(const Point& origin, const Vector& direction)
      : origin(origin), direction(direction) {}

  Ray(const Ray&) = default;

  bool operator==(const Ray&) const = default;

  Point Endpoint(geometric_t distance) const {
    assert(std::isfinite(distance));
    return origin + direction * distance;
  }

  const Point origin;
  const Vector direction;
};

Ray Normalize(const Ray& ray) {
  return Ray(ray.origin, Normalize(ray.direction));
}

}  // namespace iris

#endif  // _IRIS_RAY_