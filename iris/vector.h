#ifndef _IRIS_VECTOR_
#define _IRIS_VECTOR_

#include <cassert>
#include <cmath>

#include "iris/float.h"

namespace iris {

struct Vector {
  explicit Vector(geometric_t x, geometric_t y, geometric_t z)
      : x(x), y(y), z(z) {
    assert(std::isfinite(x));
    assert(std::isfinite(y));
    assert(std::isfinite(z));
  }

  Vector(const Vector&) = default;

  bool operator==(const Vector&) const = default;

  const geometric_t& operator[](int index) const {
    assert(index >= 0 && index < 3);
    const geometric_t* as_array = &x;
    return as_array[index];
  }

  geometric_t Length() const;

  const geometric_t x;
  const geometric_t y;
  const geometric_t z;
};

Vector operator-(const Vector& vector) {
  return Vector(-vector.x, -vector.y, -vector.z);
}

Vector operator-(const Vector& minuend, const Vector& subtrahend) {
  return Vector(minuend.x - subtrahend.x, minuend.y - subtrahend.y,
                minuend.z - subtrahend.z);
}

Vector operator+(const Vector& addend0, const Vector& addend1) {
  return Vector(addend0.x + addend1.x, addend0.y + addend1.y,
                addend0.z + addend1.z);
}

Vector operator*(const Vector& vector, geometric_t scalar) {
  assert(std::isfinite(scalar));
  return Vector(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

Vector operator*(geometric_t scalar, const Vector& vector) {
  assert(std::isfinite(scalar));
  return vector * scalar;
}

Vector operator/(const Vector& dividend, geometric_t divisor) {
  assert(!std::isnan(divisor) && divisor != static_cast<geometric_t>(0.0));
  return Vector(dividend.x / divisor, dividend.y / divisor,
                dividend.z / divisor);
}

geometric_t DotProduct(const Vector& operand0, const Vector& operand1) {
  return operand0.x * operand1.x + operand0.y * operand1.y +
         operand0.z * operand1.z;
}

Vector CrossProduct(const Vector& operand0, const Vector& operand1) {
  return Vector(operand0.y * operand1.z - operand0.z * operand1.y,
                operand0.z * operand1.x - operand0.x * operand1.z,
                operand0.x * operand1.y - operand0.y * operand1.x);
}

Vector Normalize(const Vector& vector, geometric_t* length_squared = nullptr,
                 geometric_t* length = nullptr) {
  geometric_t old_length_squared = DotProduct(vector, vector);
  if (length_squared) {
    *length_squared = old_length_squared;
  }

  geometric_t old_length = std::sqrt(old_length_squared);
  if (length) {
    *length = old_length;
  }

  return vector / old_length;
}

geometric_t Vector::Length() const {
  return std::sqrt(DotProduct(*this, *this));
}

}  // namespace iris

#endif  // _IRIS_VECTOR_