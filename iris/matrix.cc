#include "iris/matrix.h"

#include <cassert>
#include <cmath>
#include <optional>
#include <string>

namespace iris {
namespace {

std::optional<std::array<std::array<geometric_t, 4>, 4>> Invert(
    const std::array<std::array<geometric_t, 4>, 4>& m) {
  std::array<std::array<geometric_t, 4>, 4> inverse = m;

  size_t column_index[] = {0, 0, 0, 0};
  size_t row_index[] = {0, 0, 0, 0};
  size_t pivot[] = {0, 0, 0, 0};

  for (size_t i = 0; i < 4; i++) {
    float_t best_candidate = (float_t)0.0;
    size_t best_column = 0;
    size_t best_row = 0;

    for (size_t j = 0; j < 4; j++) {
      if (pivot[j] == 1) {
        continue;
      }

      for (size_t k = 0; k < 4; k++) {
        if (pivot[k] == 1) {
          continue;
        }

        if (pivot[k] > 1) {
          return std::nullopt;
        }

        float_t new_candidate = fabs(inverse[j][k]);
        if (new_candidate >= best_candidate) {
          best_candidate = new_candidate;
          best_row = j;
          best_column = k;
        }
      }
    }

    pivot[best_column] += 1;

    if (best_row != best_column) {
      for (size_t k = 0; k < 4; k++) {
        float_t tmp = inverse[best_row][k];
        inverse[best_row][k] = inverse[best_column][k];
        inverse[best_column][k] = tmp;
      }
    }

    if (inverse[best_column][best_column] == (float_t)0.0) {
      return std::nullopt;
    }

    column_index[i] = best_column;
    row_index[i] = best_row;

    float_t divisor = inverse[best_column][best_column];
    inverse[best_column][best_column] = (float_t)1.0;
    for (size_t j = 0; j < 4; j++) {
      inverse[best_column][j] /= divisor;
    }

    for (size_t j = 0; j < 4; j++) {
      if (j == best_column) {
        continue;
      }

      float_t scalar = -inverse[j][best_column];
      inverse[j][best_column] = (float_t)0.0;
      for (size_t k = 0; k < 4; k++) {
        inverse[j][k] += scalar * inverse[best_column][k];
      }
    }
  }

  for (size_t jj = 0; jj < 4; jj++) {
    size_t j = 3 - jj;
    if (row_index[j] == column_index[j]) {
      continue;
    }

    for (size_t k = 0; k < 4; k++) {
      float_t tmp = inverse[k][row_index[j]];
      inverse[k][row_index[j]] = inverse[k][column_index[j]];
      inverse[k][column_index[j]] = tmp;
    }
  }

  return inverse;
}

std::array<std::array<geometric_t, 4>, 4> Multiply4x4(
    const std::array<std::array<geometric_t, 4>, 4>& left,
    const std::array<std::array<geometric_t, 4>, 4>& right) {
  std::array<std::array<geometric_t, 4>, 4> result;
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      result[i][j] = left[i][0] * right[0][j] + left[i][1] * right[1][j] +
                     left[i][2] * right[2][j] + left[i][3] * right[3][j];
    }
  }
  return result;
}

}  // namespace

Matrix::Matrix(const std::array<std::array<geometric_t, 4>, 4>& m,
               const std::array<std::array<geometric_t, 4>, 4>& i)
    : m{m}, i{i} {
  assert(std::isfinite(m[0][0]));
  assert(std::isfinite(m[0][1]));
  assert(std::isfinite(m[0][2]));
  assert(std::isfinite(m[0][3]));
  assert(std::isfinite(m[1][0]));
  assert(std::isfinite(m[1][1]));
  assert(std::isfinite(m[1][2]));
  assert(std::isfinite(m[1][3]));
  assert(std::isfinite(m[2][0]));
  assert(std::isfinite(m[2][1]));
  assert(std::isfinite(m[2][2]));
  assert(std::isfinite(m[2][3]));
  assert(std::isfinite(m[3][0]));
  assert(std::isfinite(m[3][1]));
  assert(std::isfinite(m[3][2]));
  assert(std::isfinite(m[3][3]));
  assert(std::isfinite(i[0][0]));
  assert(std::isfinite(i[0][1]));
  assert(std::isfinite(i[0][2]));
  assert(std::isfinite(i[0][3]));
  assert(std::isfinite(i[1][0]));
  assert(std::isfinite(i[1][1]));
  assert(std::isfinite(i[1][2]));
  assert(std::isfinite(i[1][3]));
  assert(std::isfinite(i[2][0]));
  assert(std::isfinite(i[2][1]));
  assert(std::isfinite(i[2][2]));
  assert(std::isfinite(i[2][3]));
  assert(std::isfinite(i[3][0]));
  assert(std::isfinite(i[3][1]));
  assert(std::isfinite(i[3][2]));
  assert(std::isfinite(i[3][3]));

  assert(m[0][0] != 0.0 || m[0][1] != 0.0 || m[0][2] != 0.0 || m[0][3] != 0.0 ||
         m[1][0] != 0.0 || m[1][1] != 0.0 || m[1][2] != 0.0 || m[1][3] != 0.0 ||
         m[2][0] != 0.0 || m[2][1] != 0.0 || m[2][2] != 0.0 || m[2][3] != 0.0 ||
         m[3][0] != 0.0 || m[3][1] != 0.0 || m[3][2] != 0.0 || m[3][3] != 0.0 ||
         i[0][0] != 0.0 || i[0][1] != 0.0 || i[0][2] != 0.0 || i[0][3] != 0.0 ||
         i[1][0] != 0.0 || i[1][1] != 0.0 || i[1][2] != 0.0 || i[1][3] != 0.0 ||
         i[2][0] != 0.0 || i[2][1] != 0.0 || i[2][2] != 0.0 || i[2][3] != 0.0 ||
         i[3][0] != 0.0 || i[3][1] != 0.0 || i[3][2] != 0.0 || i[3][3] != 0.0);
}

absl::StatusOr<Matrix> Matrix::Create(
    const std::array<std::array<geometric_t, 4>, 4>& m) {
  for (size_t i = 0; i < m.size(); i++) {
    for (size_t j = 0; j < m[i].size(); j++) {
      if (!std::isfinite(m[i][j])) {
        return absl::InvalidArgumentError("m[" + std::to_string(i) + "][" +
                                          std::to_string(j) + "]");
      }
    }
  }

  auto inverse = Invert(m);
  if (!inverse) {
    return absl::InvalidArgumentError("Could not invert matrix");
  }

  return Matrix(m, inverse.value());
}

absl::StatusOr<Matrix> Matrix::Translation(geometric_t x, geometric_t y,
                                           geometric_t z) {
  if (!std::isfinite(x)) {
    return absl::InvalidArgumentError("x");
  }

  if (!std::isfinite(y)) {
    return absl::InvalidArgumentError("y");
  }

  if (!std::isfinite(z)) {
    return absl::InvalidArgumentError("z");
  }

  std::array<std::array<geometric_t, 4>, 4> matrix = {{{1.0, 0.0, 0.0, x},
                                                       {0.0, 1.0, 0.0, y},
                                                       {0.0, 0.0, 1.0, z},
                                                       {0.0, 0.0, 0.0, 1.0}}};
  std::array<std::array<geometric_t, 4>, 4> inverse = {{{1.0, 0.0, 0.0, -x},
                                                        {0.0, 1.0, 0.0, -y},
                                                        {0.0, 0.0, 1.0, -z},
                                                        {0.0, 0.0, 0.0, 1.0}}};

  return Matrix(matrix, inverse);
}

absl::StatusOr<Matrix> Matrix::Scalar(geometric_t x, geometric_t y,
                                      geometric_t z) {
  if (!std::isfinite(x) || x == 0.0) {
    return absl::InvalidArgumentError("x");
  }

  if (!std::isfinite(y) || y == 0.0) {
    return absl::InvalidArgumentError("y");
  }

  if (!std::isfinite(z) || z == 0.0) {
    return absl::InvalidArgumentError("z");
  }

  std::array<std::array<geometric_t, 4>, 4> matrix = {{{x, 0.0, 0.0, 0.0},
                                                       {0.0, y, 0.0, 0.0},
                                                       {0.0, 0.0, z, 0.0},
                                                       {0.0, 0.0, 0.0, 1.0}}};
  std::array<std::array<geometric_t, 4>, 4> inverse = {
      {{static_cast<geometric_t>(1.0) / x, 0.0, 0.0, 0.0},
       {0.0, static_cast<geometric_t>(1.0) / y, 0.0, 0.0},
       {0.0, 0.0, static_cast<geometric_t>(1.0) / z, 0.0},
       {0.0, 0.0, 0.0, 1.0}}};

  return Matrix(matrix, inverse);
}

absl::StatusOr<Matrix> Matrix::Rotation(geometric_t theta, geometric_t x,
                                        geometric_t y, geometric_t z) {
  if (!std::isfinite(theta)) {
    return absl::InvalidArgumentError("theta");
  }

  if (!std::isfinite(x)) {
    return absl::InvalidArgumentError("x");
  }

  if (!std::isfinite(y)) {
    return absl::InvalidArgumentError("y");
  }

  if (!std::isfinite(z)) {
    return absl::InvalidArgumentError("z");
  }

  if (x == 0.0 && y == 0.0 && z == 0.0) {
    return absl::InvalidArgumentError("One of x, y, or z must be non-zero");
  }

  Vector axis = Normalize(Vector(x, y, z));

  geometric_t sin_theta = std::sin(theta);
  geometric_t cos_theta = std::cos(theta);
  geometric_t ic = 1.0 - cos_theta;

  geometric_t m00 = axis.x * axis.x * ic + cos_theta;
  geometric_t m01 = axis.x * axis.y * ic - axis.z * sin_theta;
  geometric_t m02 = axis.x * axis.z * ic + axis.y * sin_theta;

  geometric_t a0 = axis.y * axis.x * ic + axis.z * sin_theta;
  geometric_t a1 = axis.y * axis.y * ic + cos_theta;
  geometric_t a2 = axis.y * axis.z * ic - axis.x * sin_theta;

  geometric_t b0 = axis.z * axis.x * ic - axis.y * sin_theta;
  geometric_t b1 = axis.z * axis.y * ic + axis.x * sin_theta;
  geometric_t b2 = axis.z * axis.z * ic + cos_theta;

  std::array<std::array<geometric_t, 4>, 4> matrix = {{{m00, m01, m02, 0.0},
                                                       {a0, a1, a2, 0.0},
                                                       {b0, b1, b2, 0.0},
                                                       {0.0, 0.0, 0.0, 1.0}}};
  std::array<std::array<geometric_t, 4>, 4> inverse = {{{m00, a0, b0, 0.0},
                                                        {m01, a1, b1, 0.0},
                                                        {m02, a2, b2, 0.0},
                                                        {0.0, 0.0, 0.0, 1.0}}};

  return Matrix(matrix, inverse);
}

absl::StatusOr<Matrix> Matrix::Orthographic(geometric_t left, geometric_t right,
                                            geometric_t bottom, geometric_t top,
                                            geometric_t near, geometric_t far) {
  if (!std::isfinite(left)) {
    return absl::InvalidArgumentError("left");
  }

  if (!std::isfinite(right)) {
    return absl::InvalidArgumentError("right");
  }

  if (!std::isfinite(bottom)) {
    return absl::InvalidArgumentError("bottom");
  }

  if (!std::isfinite(top)) {
    return absl::InvalidArgumentError("top");
  }

  if (!std::isfinite(near)) {
    return absl::InvalidArgumentError("near");
  }

  if (!std::isfinite(far)) {
    return absl::InvalidArgumentError("far");
  }

  if (left == right) {
    return absl::InvalidArgumentError("left cannot equal right");
  }

  if (bottom == top) {
    return absl::InvalidArgumentError("botom cannot equal top");
  }

  if (near == far) {
    return absl::InvalidArgumentError("near cannot equal far");
  }

  geometric_t tx = -(right + left) / (right - left);
  geometric_t ty = -(top + bottom) / (top - bottom);
  geometric_t tz = -(far + near) / (far - near);

  geometric_t sx = static_cast<geometric_t>(2.0) / (right - left);
  geometric_t sy = static_cast<geometric_t>(2.0) / (top - bottom);
  geometric_t sz = static_cast<geometric_t>(-2.0) / (far - near);

  std::array<std::array<geometric_t, 4>, 4> matrix = {{{sx, 0.0, 0.0, tx},
                                                       {0.0, sy, 0.0, ty},
                                                       {0.0, 0.0, sz, tz},
                                                       {0.0, 0.0, 0.0, 1.0}}};
  std::array<std::array<geometric_t, 4>, 4> inverse = {
      {{static_cast<geometric_t>(1.0) / sx, 0.0, 0.0, -tx / sx},
       {0.0, static_cast<geometric_t>(1.0) / sy, 0.0, -ty / sy},
       {0.0, 0.0, static_cast<geometric_t>(1.0) / sz, -tz / sz},
       {0.0, 0.0, 0.0, 1.0}}};

  return Matrix(matrix, inverse);
}

const Matrix& Matrix::Identity() {
  static const std::array<std::array<geometric_t, 4>, 4> identity_values = {
      {{1.0, 0.0, 0.0, 0.0},
       {0.0, 1.0, 0.0, 0.0},
       {0.0, 0.0, 1.0, 0.0},
       {0.0, 0.0, 0.0, 1.0}}};
  static const Matrix identity_matrix(identity_values, identity_values);
  return identity_matrix;
}

Matrix Matrix::Multiply(const Matrix& left, const Matrix& right) {
  return left.Multiply(right);
}

Matrix Matrix::Multiply(const Matrix& matrix) const {
  return Matrix(Multiply4x4(m, matrix.m), Multiply4x4(matrix.i, i));
}

}  // namespace iris