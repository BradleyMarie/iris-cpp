#ifndef _IRIS_MATRIX_
#define _IRIS_MATRIX_

#include <array>

#include "absl/status/statusor.h"
#include "iris/float.h"
#include "iris/point.h"
#include "iris/ray.h"
#include "iris/vector.h"

namespace iris {

struct Matrix final {
  Matrix(const Matrix& matrix) noexcept = default;

  static const Matrix& Identity();

  static absl::StatusOr<Matrix> Translation(geometric x, geometric y,
                                            geometric z);
  static absl::StatusOr<Matrix> Scalar(geometric x, geometric y, geometric z);
  static absl::StatusOr<Matrix> Rotation(geometric theta, geometric x,
                                         geometric y, geometric z);
  static absl::StatusOr<Matrix> LookAt(geometric eye_x, geometric eye_y,
                                       geometric eye_z, geometric look_at_x,
                                       geometric look_at_y, geometric look_at_z,
                                       geometric up_x, geometric up_y,
                                       geometric up_z);
  static absl::StatusOr<Matrix> Orthographic(geometric left, geometric right,
                                             geometric bottom, geometric top,
                                             geometric near, geometric far);

  static absl::StatusOr<Matrix> Create(
      const std::array<std::array<geometric, 4>, 4>& m);

  Point Multiply(const Point& point) const;
  Point InverseMultiply(const Point& point) const;

  Ray Multiply(const Ray& ray) const;
  Ray InverseMultiply(const Ray& ray) const;

  Vector Multiply(const Vector& vector) const;
  Vector InverseMultiply(const Vector& vector) const;
  Vector InverseTransposeMultiply(const Vector& vector) const;
  Vector TransposeMultiply(const Vector& vector) const;

  static Matrix Multiply(const Matrix& left, const Matrix& right);
  Matrix Multiply(const Matrix& matrix) const;

  const std::array<geometric, 4>& operator[](size_t index) const {
    return m[index];
  }

  bool operator==(const Matrix& matrix) const = default;

 private:
  Matrix(const std::array<std::array<geometric, 4>, 4>& m,
         const std::array<std::array<geometric, 4>, 4>& i);

 public:
  const std::array<std::array<geometric, 4>, 4> m;
  const std::array<std::array<geometric, 4>, 4> i;
};

bool operator<(const Matrix& left, const Matrix& right);

inline Point Matrix::Multiply(const Point& point) const {
  return Point(
      m[0][0] * point.x + m[0][1] * point.y + m[0][2] * point.z + m[0][3],
      m[1][0] * point.x + m[1][1] * point.y + m[1][2] * point.z + m[1][3],
      m[2][0] * point.x + m[2][1] * point.y + m[2][2] * point.z + m[2][3]);
}

inline Point Matrix::InverseMultiply(const Point& point) const {
  return Point(
      i[0][0] * point.x + i[0][1] * point.y + i[0][2] * point.z + i[0][3],
      i[1][0] * point.x + i[1][1] * point.y + i[1][2] * point.z + i[1][3],
      i[2][0] * point.x + i[2][1] * point.y + i[2][2] * point.z + i[2][3]);
}

inline Ray Matrix::Multiply(const Ray& ray) const {
  return Ray(Multiply(ray.origin), Multiply(ray.direction));
}

inline Ray Matrix::InverseMultiply(const Ray& ray) const {
  return Ray(InverseMultiply(ray.origin), InverseMultiply(ray.direction));
}

inline Vector Matrix::Multiply(const Vector& vector) const {
  return Vector(m[0][0] * vector.x + m[0][1] * vector.y + m[0][2] * vector.z,
                m[1][0] * vector.x + m[1][1] * vector.y + m[1][2] * vector.z,
                m[2][0] * vector.x + m[2][1] * vector.y + m[2][2] * vector.z);
}

inline Vector Matrix::InverseMultiply(const Vector& vector) const {
  return Vector(i[0][0] * vector.x + i[0][1] * vector.y + i[0][2] * vector.z,
                i[1][0] * vector.x + i[1][1] * vector.y + i[1][2] * vector.z,
                i[2][0] * vector.x + i[2][1] * vector.y + i[2][2] * vector.z);
}

inline Vector Matrix::InverseTransposeMultiply(const Vector& vector) const {
  return Vector(i[0][0] * vector.x + i[1][0] * vector.y + i[2][0] * vector.z,
                i[0][1] * vector.x + i[1][1] * vector.y + i[2][1] * vector.z,
                i[0][2] * vector.x + i[1][2] * vector.y + i[2][2] * vector.z);
}

inline Vector Matrix::TransposeMultiply(const Vector& vector) const {
  return Vector(m[0][0] * vector.x + m[1][0] * vector.y + m[2][0] * vector.z,
                m[0][1] * vector.x + m[1][1] * vector.y + m[2][1] * vector.z,
                m[0][2] * vector.x + m[1][2] * vector.y + m[2][2] * vector.z);
}

}  // namespace iris

#endif  // _IRIS_MATRIX_