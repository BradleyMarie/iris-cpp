#include "iris/matrix.h"

#include <limits>

#include "googletest/include/gtest/gtest.h"

TEST(MatrixTest, Identity) {
  const auto& matrix = iris::Matrix::Identity();
  EXPECT_EQ(1.0, matrix.m[0][0]);
  EXPECT_EQ(0.0, matrix.m[0][1]);
  EXPECT_EQ(0.0, matrix.m[0][2]);
  EXPECT_EQ(0.0, matrix.m[0][3]);
  EXPECT_EQ(0.0, matrix.m[1][0]);
  EXPECT_EQ(1.0, matrix.m[1][1]);
  EXPECT_EQ(0.0, matrix.m[1][2]);
  EXPECT_EQ(0.0, matrix.m[1][3]);
  EXPECT_EQ(0.0, matrix.m[2][0]);
  EXPECT_EQ(0.0, matrix.m[2][1]);
  EXPECT_EQ(1.0, matrix.m[2][2]);
  EXPECT_EQ(0.0, matrix.m[2][3]);
  EXPECT_EQ(0.0, matrix.m[3][0]);
  EXPECT_EQ(0.0, matrix.m[3][1]);
  EXPECT_EQ(0.0, matrix.m[3][2]);
  EXPECT_EQ(1.0, matrix.m[3][3]);

  EXPECT_EQ(1.0, matrix.i[0][0]);
  EXPECT_EQ(0.0, matrix.i[0][1]);
  EXPECT_EQ(0.0, matrix.i[0][2]);
  EXPECT_EQ(0.0, matrix.i[0][3]);
  EXPECT_EQ(0.0, matrix.i[1][0]);
  EXPECT_EQ(1.0, matrix.i[1][1]);
  EXPECT_EQ(0.0, matrix.i[1][2]);
  EXPECT_EQ(0.0, matrix.i[1][3]);
  EXPECT_EQ(0.0, matrix.i[2][0]);
  EXPECT_EQ(0.0, matrix.i[2][1]);
  EXPECT_EQ(1.0, matrix.i[2][2]);
  EXPECT_EQ(0.0, matrix.i[2][3]);
  EXPECT_EQ(0.0, matrix.i[3][0]);
  EXPECT_EQ(0.0, matrix.i[3][1]);
  EXPECT_EQ(0.0, matrix.i[3][2]);
  EXPECT_EQ(1.0, matrix.i[3][3]);
}

TEST(MatrixTest, TranslationErrors) {
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Translation(
          std::numeric_limits<iris::geometric_t>::infinity(), 1.0, 1.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Translation(
          1.0, std::numeric_limits<iris::geometric_t>::infinity(), 1.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Translation(
          1.0, 1.0, std::numeric_limits<iris::geometric_t>::infinity())
          .status()));
}

TEST(MatrixTest, Translation) {
  auto matrix = iris::Matrix::Translation(1.0, 2.0, 3.0).value();
  EXPECT_EQ(1.0, matrix.m[0][0]);
  EXPECT_EQ(0.0, matrix.m[0][1]);
  EXPECT_EQ(0.0, matrix.m[0][2]);
  EXPECT_EQ(1.0, matrix.m[0][3]);
  EXPECT_EQ(0.0, matrix.m[1][0]);
  EXPECT_EQ(1.0, matrix.m[1][1]);
  EXPECT_EQ(0.0, matrix.m[1][2]);
  EXPECT_EQ(2.0, matrix.m[1][3]);
  EXPECT_EQ(0.0, matrix.m[2][0]);
  EXPECT_EQ(0.0, matrix.m[2][1]);
  EXPECT_EQ(1.0, matrix.m[2][2]);
  EXPECT_EQ(3.0, matrix.m[2][3]);
  EXPECT_EQ(0.0, matrix.m[3][0]);
  EXPECT_EQ(0.0, matrix.m[3][1]);
  EXPECT_EQ(0.0, matrix.m[3][2]);
  EXPECT_EQ(1.0, matrix.m[3][3]);

  EXPECT_EQ(1.0, matrix.i[0][0]);
  EXPECT_EQ(0.0, matrix.i[0][1]);
  EXPECT_EQ(0.0, matrix.i[0][2]);
  EXPECT_EQ(-1.0, matrix.i[0][3]);
  EXPECT_EQ(0.0, matrix.i[1][0]);
  EXPECT_EQ(1.0, matrix.i[1][1]);
  EXPECT_EQ(0.0, matrix.i[1][2]);
  EXPECT_EQ(-2.0, matrix.i[1][3]);
  EXPECT_EQ(0.0, matrix.i[2][0]);
  EXPECT_EQ(0.0, matrix.i[2][1]);
  EXPECT_EQ(1.0, matrix.i[2][2]);
  EXPECT_EQ(-3.0, matrix.i[2][3]);
  EXPECT_EQ(0.0, matrix.i[3][0]);
  EXPECT_EQ(0.0, matrix.i[3][1]);
  EXPECT_EQ(0.0, matrix.i[3][2]);
  EXPECT_EQ(1.0, matrix.i[3][3]);
}

TEST(MatrixTest, ScalarErrors) {
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Scalar(std::numeric_limits<iris::geometric_t>::infinity(),
                           1.0, 1.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Scalar(
          1.0, std::numeric_limits<iris::geometric_t>::infinity(), 1.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Scalar(1.0, 1.0,
                           std::numeric_limits<iris::geometric_t>::infinity())
          .status()));
  EXPECT_TRUE(
      absl::IsInvalidArgument(iris::Matrix::Scalar(0.0, 1.0, 1.0).status()));
  EXPECT_TRUE(
      absl::IsInvalidArgument(iris::Matrix::Scalar(1.0, 0.0, 1.0).status()));
  EXPECT_TRUE(
      absl::IsInvalidArgument(iris::Matrix::Scalar(1.0, 1.0, 0.0).status()));
}

TEST(MatrixTest, Scalar) {
  auto matrix = iris::Matrix::Scalar(2.0, 4.0, 8.0).value();
  EXPECT_EQ(2.0, matrix.m[0][0]);
  EXPECT_EQ(0.0, matrix.m[0][1]);
  EXPECT_EQ(0.0, matrix.m[0][2]);
  EXPECT_EQ(0.0, matrix.m[0][3]);
  EXPECT_EQ(0.0, matrix.m[1][0]);
  EXPECT_EQ(4.0, matrix.m[1][1]);
  EXPECT_EQ(0.0, matrix.m[1][2]);
  EXPECT_EQ(0.0, matrix.m[1][3]);
  EXPECT_EQ(0.0, matrix.m[2][0]);
  EXPECT_EQ(0.0, matrix.m[2][1]);
  EXPECT_EQ(8.0, matrix.m[2][2]);
  EXPECT_EQ(0.0, matrix.m[2][3]);
  EXPECT_EQ(0.0, matrix.m[3][0]);
  EXPECT_EQ(0.0, matrix.m[3][1]);
  EXPECT_EQ(0.0, matrix.m[3][2]);
  EXPECT_EQ(1.0, matrix.m[3][3]);

  EXPECT_EQ(0.5, matrix.i[0][0]);
  EXPECT_EQ(0.0, matrix.i[0][1]);
  EXPECT_EQ(0.0, matrix.i[0][2]);
  EXPECT_EQ(0.0, matrix.i[0][3]);
  EXPECT_EQ(0.0, matrix.i[1][0]);
  EXPECT_EQ(0.25, matrix.i[1][1]);
  EXPECT_EQ(0.0, matrix.i[1][2]);
  EXPECT_EQ(0.0, matrix.i[1][3]);
  EXPECT_EQ(0.0, matrix.i[2][0]);
  EXPECT_EQ(0.0, matrix.i[2][1]);
  EXPECT_EQ(0.125, matrix.i[2][2]);
  EXPECT_EQ(0.0, matrix.i[2][3]);
  EXPECT_EQ(0.0, matrix.i[3][0]);
  EXPECT_EQ(0.0, matrix.i[3][1]);
  EXPECT_EQ(0.0, matrix.i[3][2]);
  EXPECT_EQ(1.0, matrix.i[3][3]);
}

TEST(MatrixTest, RotationErrors) {
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Rotation(std::numeric_limits<iris::geometric_t>::infinity(),
                             1.0, 1.0, 1.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Rotation(
          1.0, std::numeric_limits<iris::geometric_t>::infinity(), 1.0, 1.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Rotation(
          1.0, 1.0, std::numeric_limits<iris::geometric_t>::infinity(), 1.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Rotation(1.0, 1.0, 1.0,
                             std::numeric_limits<iris::geometric_t>::infinity())
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Rotation(1.0, 0.0, 0.0, 0.0).status()));
}

TEST(MatrixTest, Rotation) {
  auto matrix = iris::Matrix::Rotation(4.0, 1.0, 2.0, 3.0).value();
  EXPECT_NEAR(static_cast<iris::geometric_t>(-0.5355262), matrix.m[0][0],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.8430267), matrix.m[0][1],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(-0.0501757), matrix.m[0][2],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_EQ(0.0, matrix.m[0][3]);
  EXPECT_NEAR(static_cast<iris::geometric_t>(-0.3705571), matrix.m[1][0],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(-0.181174), matrix.m[1][1],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.9109684), matrix.m[1][2],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_EQ(0.0, matrix.m[1][3]);
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.7588801), matrix.m[2][0],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.5064405), matrix.m[2][1],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.4094130), matrix.m[2][2],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_EQ(0.0, matrix.m[2][3]);
  EXPECT_EQ(0.0, matrix.m[3][0]);
  EXPECT_EQ(0.0, matrix.m[3][1]);
  EXPECT_EQ(0.0, matrix.m[3][2]);
  EXPECT_EQ(1.0, matrix.m[3][3]);

  EXPECT_NEAR(static_cast<iris::geometric_t>(-0.5355262), matrix.i[0][0],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.8430267), matrix.i[1][0],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(-0.0501757), matrix.i[2][0],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_EQ(0.0, matrix.m[3][0]);
  EXPECT_NEAR(static_cast<iris::geometric_t>(-0.3705571), matrix.i[0][1],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(-0.181174), matrix.i[1][1],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.9109684), matrix.i[2][1],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_EQ(0.0, matrix.m[3][1]);
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.7588801), matrix.i[0][2],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.5064405), matrix.i[1][2],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(static_cast<iris::geometric_t>(0.4094130), matrix.i[2][2],
              static_cast<iris::geometric_t>(0.0001));
  EXPECT_EQ(0.0, matrix.m[3][2]);
  EXPECT_EQ(0.0, matrix.m[0][3]);
  EXPECT_EQ(0.0, matrix.m[1][3]);
  EXPECT_EQ(0.0, matrix.m[2][3]);
  EXPECT_EQ(1.0, matrix.m[3][3]);
}

TEST(MatrixTest, OrthographicErrors) {
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Orthographic(
          std::numeric_limits<iris::geometric_t>::infinity(), 2.0, 1.0, 2.0,
          1.0, 2.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Orthographic(
          1.0, std::numeric_limits<iris::geometric_t>::infinity(), 1.0, 2.0,
          1.0, 2.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Orthographic(
          1.0, 2.0, std::numeric_limits<iris::geometric_t>::infinity(), 2.0,
          1.0, 2.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Orthographic(
          1.0, 2.0, 1.0, std::numeric_limits<iris::geometric_t>::infinity(),
          1.0, 2.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Orthographic(
          1.0, 2.0, 1.0, 2.0,
          std::numeric_limits<iris::geometric_t>::infinity(), 2.0)
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Orthographic(
          1.0, 2.0, 1.0, 2.0, 1.0,
          std::numeric_limits<iris::geometric_t>::infinity())
          .status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Orthographic(1.0, 1.0, 1.0, 2.0, 1.0, 2.0).status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Orthographic(1.0, 2.0, 1.0, 1.0, 1.0, 2.0).status()));
  EXPECT_TRUE(absl::IsInvalidArgument(
      iris::Matrix::Orthographic(1.0, 2.0, 1.0, 2.0, 1.0, 1.0).status()));
}

TEST(MatrixTest, Orthographic) {
  auto matrix =
      iris::Matrix::Orthographic(1.0, 2.0, 2.0, 4.0, 4.0, 8.0).value();
  EXPECT_EQ(2.0, matrix.m[0][0]);
  EXPECT_EQ(0.0, matrix.m[0][1]);
  EXPECT_EQ(0.0, matrix.m[0][2]);
  EXPECT_EQ(-3.0, matrix.m[0][3]);
  EXPECT_EQ(0.0, matrix.m[1][0]);
  EXPECT_EQ(1.0, matrix.m[1][1]);
  EXPECT_EQ(0.0, matrix.m[1][2]);
  EXPECT_EQ(-3.0, matrix.m[1][3]);
  EXPECT_EQ(0.0, matrix.m[2][0]);
  EXPECT_EQ(0.0, matrix.m[2][1]);
  EXPECT_EQ(-0.5, matrix.m[2][2]);
  EXPECT_EQ(-3.0, matrix.m[2][3]);
  EXPECT_EQ(0.0, matrix.m[3][0]);
  EXPECT_EQ(0.0, matrix.m[3][1]);
  EXPECT_EQ(0.0, matrix.m[3][2]);
  EXPECT_EQ(1.0, matrix.m[3][3]);

  EXPECT_EQ(0.5, matrix.i[0][0]);
  EXPECT_EQ(0.0, matrix.i[0][1]);
  EXPECT_EQ(0.0, matrix.i[0][2]);
  EXPECT_EQ(1.5, matrix.i[0][3]);
  EXPECT_EQ(0.0, matrix.i[1][0]);
  EXPECT_EQ(1.0, matrix.i[1][1]);
  EXPECT_EQ(0.0, matrix.i[1][2]);
  EXPECT_EQ(3.0, matrix.i[1][3]);
  EXPECT_EQ(0.0, matrix.i[2][0]);
  EXPECT_EQ(0.0, matrix.i[2][1]);
  EXPECT_EQ(-2.0, matrix.i[2][2]);
  EXPECT_EQ(-6.0, matrix.i[2][3]);
  EXPECT_EQ(0.0, matrix.i[3][0]);
  EXPECT_EQ(0.0, matrix.i[3][1]);
  EXPECT_EQ(0.0, matrix.i[3][2]);
  EXPECT_EQ(1.0, matrix.i[3][3]);
}

TEST(MatrixTest, CreateErrors) {
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      std::array<std::array<iris::geometric_t, 4>, 4> matrix = {
          {{1.0, 0.0, 0.0, 0.0},
           {0.0, 1.0, 0.0, 0.0},
           {0.0, 0.0, 1.0, 0.0},
           {0.0, 0.0, 0.0, 1.0}}};
      matrix[i][j] = std::numeric_limits<iris::geometric_t>::infinity();
      EXPECT_TRUE(
          absl::IsInvalidArgument(iris::Matrix::Create(matrix).status()));
    }
  }

  std::array<std::array<iris::geometric_t, 4>, 4> matrix = {
      {{0.0, 0.0, 0.0, 0.0},
       {0.0, 0.0, 0.0, 0.0},
       {0.0, 0.0, 0.0, 0.0},
       {0.0, 0.0, 0.0, 0.0}}};
  EXPECT_TRUE(absl::IsInvalidArgument(iris::Matrix::Create(matrix).status()));
}

TEST(MatrixTest, Create) {
  std::array<std::array<iris::geometric_t, 4>, 4> ortho = {
      {{2.0, 0.0, 0.0, -3.0},
       {0.0, 1.0, 0.0, -3.0},
       {0.0, 0.0, -0.5, -3.0},
       {0.0, 0.0, 0.0, 1.0}}};
  auto matrix = iris::Matrix::Create(ortho).value();

  EXPECT_EQ(2.0, matrix.m[0][0]);
  EXPECT_EQ(0.0, matrix.m[0][1]);
  EXPECT_EQ(0.0, matrix.m[0][2]);
  EXPECT_EQ(-3.0, matrix.m[0][3]);
  EXPECT_EQ(0.0, matrix.m[1][0]);
  EXPECT_EQ(1.0, matrix.m[1][1]);
  EXPECT_EQ(0.0, matrix.m[1][2]);
  EXPECT_EQ(-3.0, matrix.m[1][3]);
  EXPECT_EQ(0.0, matrix.m[2][0]);
  EXPECT_EQ(0.0, matrix.m[2][1]);
  EXPECT_EQ(-0.5, matrix.m[2][2]);
  EXPECT_EQ(-3.0, matrix.m[2][3]);
  EXPECT_EQ(0.0, matrix.m[3][0]);
  EXPECT_EQ(0.0, matrix.m[3][1]);
  EXPECT_EQ(0.0, matrix.m[3][2]);
  EXPECT_EQ(1.0, matrix.m[3][3]);

  EXPECT_EQ(0.5, matrix.i[0][0]);
  EXPECT_EQ(0.0, matrix.i[0][1]);
  EXPECT_EQ(0.0, matrix.i[0][2]);
  EXPECT_EQ(1.5, matrix.i[0][3]);
  EXPECT_EQ(0.0, matrix.i[1][0]);
  EXPECT_EQ(1.0, matrix.i[1][1]);
  EXPECT_EQ(0.0, matrix.i[1][2]);
  EXPECT_EQ(3.0, matrix.i[1][3]);
  EXPECT_EQ(0.0, matrix.i[2][0]);
  EXPECT_EQ(0.0, matrix.i[2][1]);
  EXPECT_EQ(-2.0, matrix.i[2][2]);
  EXPECT_EQ(-6.0, matrix.i[2][3]);
  EXPECT_EQ(0.0, matrix.i[3][0]);
  EXPECT_EQ(0.0, matrix.i[3][1]);
  EXPECT_EQ(0.0, matrix.i[3][2]);
  EXPECT_EQ(1.0, matrix.i[3][3]);
}

TEST(MatrixTest, MultiplyMatrix) {
  auto matrix = iris::Matrix::Multiply(
      iris::Matrix::Translation(-3.0, -3.0, -3.0).value(),
      iris::Matrix::Scalar(2.0, 1.0, -0.5).value());

  EXPECT_EQ(2.0, matrix.m[0][0]);
  EXPECT_EQ(0.0, matrix.m[0][1]);
  EXPECT_EQ(0.0, matrix.m[0][2]);
  EXPECT_EQ(-3.0, matrix.m[0][3]);
  EXPECT_EQ(0.0, matrix.m[1][0]);
  EXPECT_EQ(1.0, matrix.m[1][1]);
  EXPECT_EQ(0.0, matrix.m[1][2]);
  EXPECT_EQ(-3.0, matrix.m[1][3]);
  EXPECT_EQ(0.0, matrix.m[2][0]);
  EXPECT_EQ(0.0, matrix.m[2][1]);
  EXPECT_EQ(-0.5, matrix.m[2][2]);
  EXPECT_EQ(-3.0, matrix.m[2][3]);
  EXPECT_EQ(0.0, matrix.m[3][0]);
  EXPECT_EQ(0.0, matrix.m[3][1]);
  EXPECT_EQ(0.0, matrix.m[3][2]);
  EXPECT_EQ(1.0, matrix.m[3][3]);

  EXPECT_EQ(0.5, matrix.i[0][0]);
  EXPECT_EQ(0.0, matrix.i[0][1]);
  EXPECT_EQ(0.0, matrix.i[0][2]);
  EXPECT_EQ(1.5, matrix.i[0][3]);
  EXPECT_EQ(0.0, matrix.i[1][0]);
  EXPECT_EQ(1.0, matrix.i[1][1]);
  EXPECT_EQ(0.0, matrix.i[1][2]);
  EXPECT_EQ(3.0, matrix.i[1][3]);
  EXPECT_EQ(0.0, matrix.i[2][0]);
  EXPECT_EQ(0.0, matrix.i[2][1]);
  EXPECT_EQ(-2.0, matrix.i[2][2]);
  EXPECT_EQ(-6.0, matrix.i[2][3]);
  EXPECT_EQ(0.0, matrix.i[3][0]);
  EXPECT_EQ(0.0, matrix.i[3][1]);
  EXPECT_EQ(0.0, matrix.i[3][2]);
  EXPECT_EQ(1.0, matrix.i[3][3]);
}

TEST(MatrixTest, Subscript) {
  std::array<std::array<iris::geometric_t, 4>, 4> ortho = {
      {{2.0, 0.0, 0.0, -3.0},
       {0.0, 1.0, 0.0, -3.0},
       {0.0, 0.0, -0.5, -3.0},
       {0.0, 0.0, 0.0, 1.0}}};
  auto matrix = iris::Matrix::Create(ortho).value();

  EXPECT_EQ(matrix.m[0][0], matrix[0][0]);
  EXPECT_EQ(matrix.m[0][1], matrix[0][1]);
  EXPECT_EQ(matrix.m[0][2], matrix[0][2]);
  EXPECT_EQ(matrix.m[0][3], matrix[0][3]);
  EXPECT_EQ(matrix.m[1][0], matrix[1][0]);
  EXPECT_EQ(matrix.m[1][1], matrix[1][1]);
  EXPECT_EQ(matrix.m[1][2], matrix[1][2]);
  EXPECT_EQ(matrix.m[1][3], matrix[1][3]);
  EXPECT_EQ(matrix.m[2][0], matrix[2][0]);
  EXPECT_EQ(matrix.m[2][1], matrix[2][1]);
  EXPECT_EQ(matrix.m[2][2], matrix[2][2]);
  EXPECT_EQ(matrix.m[2][3], matrix[2][3]);
  EXPECT_EQ(matrix.m[3][0], matrix[3][0]);
  EXPECT_EQ(matrix.m[3][1], matrix[3][1]);
  EXPECT_EQ(matrix.m[3][2], matrix[3][2]);
  EXPECT_EQ(matrix.m[3][3], matrix[3][3]);
}

TEST(MatrixTest, MultiplyPoint) {
  auto matrix = iris::Matrix::Translation(1.0, 2.0, 3.0).value();
  EXPECT_EQ(iris::Point(1.0, 2.0, 3.0),
            matrix.Multiply(iris::Point(0.0, 0.0, 0.0)));
}

TEST(MatrixTest, InverseMultiplyPoint) {
  auto matrix = iris::Matrix::Translation(1.0, 2.0, 3.0).value();
  EXPECT_EQ(iris::Point(-1.0, -2.0, -3.0),
            matrix.InverseMultiply(iris::Point(0.0, 0.0, 0.0)));
}

TEST(MatrixTest, MultiplyVector) {
  iris::geometric_t half_pi = std::acos(0.0);
  auto rotate_x = iris::Matrix::Rotation(half_pi, 1.0, 0.0, 0.0)
                      .value()
                      .Multiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(1.0, rotate_x.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(-1.0, rotate_x.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_x.z, static_cast<iris::geometric_t>(0.0001));

  auto rotate_y = iris::Matrix::Rotation(half_pi, 0.0, 1.0, 0.0)
                      .value()
                      .Multiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(1.0, rotate_y.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_y.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(-1.0, rotate_y.z, static_cast<iris::geometric_t>(0.0001));

  auto rotate_z = iris::Matrix::Rotation(half_pi, 0.0, 0.0, 1.0)
                      .value()
                      .Multiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(-1.0, rotate_z.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_z.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_z.z, static_cast<iris::geometric_t>(0.0001));
}

TEST(MatrixTest, InverseMultiplyVector) {
  iris::geometric_t half_pi = std::acos(0.0);
  auto rotate_x = iris::Matrix::Rotation(half_pi, 1.0, 0.0, 0.0)
                      .value()
                      .InverseMultiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(1.0, rotate_x.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_x.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(-1.0, rotate_x.z, static_cast<iris::geometric_t>(0.0001));

  auto rotate_y = iris::Matrix::Rotation(half_pi, 0.0, 1.0, 0.0)
                      .value()
                      .InverseMultiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(-1.0, rotate_y.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_y.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_y.z, static_cast<iris::geometric_t>(0.0001));

  auto rotate_z = iris::Matrix::Rotation(half_pi, 0.0, 0.0, 1.0)
                      .value()
                      .InverseMultiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(1.0, rotate_z.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(-1.0, rotate_z.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_z.z, static_cast<iris::geometric_t>(0.0001));
}

TEST(MatrixTest, TransposeMultiplyVector) {
  iris::geometric_t half_pi = std::acos(0.0);
  auto rotate_x = iris::Matrix::Rotation(half_pi, 1.0, 0.0, 0.0)
                      .value()
                      .TransposeMultiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(1.0, rotate_x.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_x.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(-1.0, rotate_x.z, static_cast<iris::geometric_t>(0.0001));

  auto rotate_y = iris::Matrix::Rotation(half_pi, 0.0, 1.0, 0.0)
                      .value()
                      .TransposeMultiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(-1.0, rotate_y.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_y.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_y.z, static_cast<iris::geometric_t>(0.0001));

  auto rotate_z = iris::Matrix::Rotation(half_pi, 0.0, 0.0, 1.0)
                      .value()
                      .TransposeMultiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(1.0, rotate_z.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(-1.0, rotate_z.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_z.z, static_cast<iris::geometric_t>(0.0001));
}

TEST(MatrixTest, InverseTransposeMultiplyVector) {
  iris::geometric_t half_pi = std::acos(0.0);
  auto rotate_x = iris::Matrix::Rotation(half_pi, 1.0, 0.0, 0.0)
                      .value()
                      .InverseTransposeMultiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(1.0, rotate_x.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(-1.0, rotate_x.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_x.z, static_cast<iris::geometric_t>(0.0001));

  auto rotate_y = iris::Matrix::Rotation(half_pi, 0.0, 1.0, 0.0)
                      .value()
                      .InverseTransposeMultiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(1.0, rotate_y.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_y.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(-1.0, rotate_y.z, static_cast<iris::geometric_t>(0.0001));

  auto rotate_z = iris::Matrix::Rotation(half_pi, 0.0, 0.0, 1.0)
                      .value()
                      .InverseTransposeMultiply(iris::Vector(1.0, 1.0, 1.0));
  EXPECT_NEAR(-1.0, rotate_z.x, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_z.y, static_cast<iris::geometric_t>(0.0001));
  EXPECT_NEAR(1.0, rotate_z.z, static_cast<iris::geometric_t>(0.0001));
}

TEST(MatrixTest, LessThan) {
  auto left = iris::Matrix::Scalar(1.0, 1.0, 1.0).value();
  EXPECT_FALSE(left < left);

  auto right = iris::Matrix::Scalar(2.0, 1.0, 1.0).value();
  EXPECT_FALSE(right < left);
  EXPECT_TRUE(left < right);
}