#ifndef _IRIS_VECTOR_
#define _IRIS_VECTOR_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>

#include "iris/float.h"

namespace iris {

struct Vector final {
  enum Axis {
    X_AXIS = 0,
    Y_AXIS = 1,
    Z_AXIS = 2,
  };

  explicit Vector(geometric x, geometric y, geometric z) noexcept
      : x(x), y(y), z(z) {
    assert(std::isfinite(x));
    assert(std::isfinite(y));
    assert(std::isfinite(z));
  }

  Vector(const Vector&) noexcept = default;

  bool operator==(const Vector&) const = default;

  const geometric& operator[](size_t index) const {
    assert(index < 3);
    const geometric* as_array = &x;
    return as_array[index];
  }

  Axis DiminishedAxis() const;
  Axis DominantAxis() const;

  geometric_t Length() const;

  const geometric x;
  const geometric y;
  const geometric z;
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

template <typename T>
Vector operator*(const Vector& vector,
                 T scalar) requires std::is_floating_point<T>::value {
  assert(std::isfinite(scalar));
  return Vector(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

template <typename T>
Vector operator*(
    T scalar, const Vector& vector) requires std::is_floating_point<T>::value {
  assert(std::isfinite(scalar));
  return vector * scalar;
}

template <typename T>
Vector operator/(const Vector& dividend,
                 T divisor) requires std::is_floating_point<T>::value {
  assert(!std::isnan(divisor) && divisor != 0.0);
  return Vector(dividend.x / divisor, dividend.y / divisor,
                dividend.z / divisor);
}

geometric_t DotProduct(const Vector& operand0, const Vector& operand1) {
  return operand0.x * operand1.x + operand0.y * operand1.y +
         operand0.z * operand1.z;
}

geometric_t AbsDotProduct(const Vector& operand0, const Vector& operand1) {
  return std::abs(DotProduct(operand0, operand1));
}

geometric_t PositiveDotProduct(const Vector& operand0, const Vector& operand1) {
  return std::max(static_cast<geometric_t>(0.0),
                  DotProduct(operand0, operand1));
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

Vector::Axis Vector::DiminishedAxis() const {
  Axis smallest_axis = (std::abs(x) <= std::abs(y)) ? X_AXIS : Y_AXIS;
  return std::abs((*this)[smallest_axis]) <= std::abs(z) ? smallest_axis
                                                         : Z_AXIS;
}

Vector::Axis Vector::DominantAxis() const {
  Axis largest_axis = (std::abs(x) >= std::abs(y)) ? X_AXIS : Y_AXIS;
  return std::abs((*this)[largest_axis]) >= std::abs(z) ? largest_axis : Z_AXIS;
}

geometric_t Vector::Length() const {
  return std::sqrt(DotProduct(*this, *this));
}

}  // namespace iris

#endif  // _IRIS_VECTOR_