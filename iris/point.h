#ifndef _IRIS_POINT_
#define _IRIS_POINT_

#include <cassert>
#include <cmath>

#include "iris/float.h"
#include "iris/vector.h"

namespace iris {

struct Point final {
  explicit Point(geometric x, geometric y, geometric z) : x(x), y(y), z(z) {
    assert(std::isfinite(x));
    assert(std::isfinite(y));
    assert(std::isfinite(z));
  }

  Point(const Point&) = default;

  bool operator==(const Point&) const = default;

  const geometric& operator[](size_t index) const {
    assert(index < 3);
    const geometric* as_array = &x;
    return as_array[index];
  }

  const geometric x;
  const geometric y;
  const geometric z;
};

Vector operator-(const Point& minuend, const Point& subtrahend) {
  return Vector(minuend.x - subtrahend.x, minuend.y - subtrahend.y,
                minuend.z - subtrahend.z);
}

Point operator-(const Point& minuend, const Vector& subtrahend) {
  return Point(minuend.x - subtrahend.x, minuend.y - subtrahend.y,
               minuend.z - subtrahend.z);
}

Point operator+(const Point& addend0, const Vector& addend1) {
  return Point(addend0.x + addend1.x, addend0.y + addend1.y,
               addend0.z + addend1.z);
}

}  // namespace iris

#endif  // _IRIS_POINT_