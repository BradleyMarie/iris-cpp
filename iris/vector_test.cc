#include "iris/vector.h"

#include "googletest/include/gtest/gtest.h"

TEST(VectorTest, Create) {
  auto vector = iris::Vector(1.0, 2.0, 3.0);
  EXPECT_EQ(1.0, vector.x);
  EXPECT_EQ(2.0, vector.y);
  EXPECT_EQ(3.0, vector.z);
}

TEST(VectorTest, Subscript) {
  auto vector = iris::Vector(1.0, 2.0, 3.0);
  EXPECT_EQ(1.0, vector[0]);
  EXPECT_EQ(2.0, vector[1]);
  EXPECT_EQ(3.0, vector[2]);
}

TEST(VectorTest, Length) {
  auto vector0 = iris::Vector(2.0, 0.0, 0.0);
  EXPECT_EQ(2.0, vector0.Length());

  auto vector1 = iris::Vector(0.0, 2.0, 0.0);
  EXPECT_EQ(2.0, vector1.Length());

  auto vector2 = iris::Vector(0.0, 0.0, 2.0);
  EXPECT_EQ(2.0, vector2.Length());
}

TEST(VectorTest, Negate) {
  auto vector = iris::Vector(1.0, 2.0, 3.0);
  EXPECT_EQ(iris::Vector(-1.0, -2.0, -3.0), -vector);
}

TEST(VectorTest, Subtract) {
  auto minuend = iris::Vector(3.0, 4.0, 5.0);
  auto subtrahend = iris::Vector(2.0, 1.0, 3.0);
  EXPECT_EQ(iris::Vector(1.0, 3.0, 2.0), minuend - subtrahend);
}

TEST(VectorTest, Add) {
  auto addend0 = iris::Vector(3.0, 4.0, 5.0);
  auto addend1 = iris::Vector(2.0, 2.0, 3.0);
  EXPECT_EQ(iris::Vector(5.0, 6.0, 8.0), addend0 + addend1);
}

TEST(VectorTest, Scale) {
  auto vector = iris::Vector(1.0, 2.0, 3.0);
  iris::geometric scalar = 2.0;
  EXPECT_EQ(iris::Vector(2.0, 4.0, 6.0), vector * scalar);
  EXPECT_EQ(iris::Vector(2.0, 4.0, 6.0), scalar * vector);
}

TEST(VectorTest, Divide) {
  auto dividend = iris::Vector(2.0, 4.0, 6.0);
  iris::geometric divisor = 2.0;
  EXPECT_EQ(iris::Vector(1.0, 2.0, 3.0), dividend / divisor);
}

TEST(VectorTest, DotProduct) {
  auto operand0 = iris::Vector(1.0, 2.0, 3.0);
  auto operand1 = iris::Vector(3.0, 4.0, 5.0);
  EXPECT_EQ(26.0, iris::DotProduct(operand0, operand1));
}

TEST(VectorTest, PositiveDotProduct) {
  auto operand0 = iris::Vector(1.0, 2.0, 3.0);
  auto operand1 = iris::Vector(3.0, 4.0, 5.0);
  EXPECT_EQ(26.0, iris::PositiveDotProduct(operand0, operand1));
  EXPECT_EQ(0.0, iris::PositiveDotProduct(operand0, -operand1));
}

TEST(VectorTest, CrossProduct) {
  auto operand0 = iris::Vector(1.0, 2.0, 3.0);
  auto operand1 = iris::Vector(3.0, 2.0, 1.0);
  EXPECT_EQ(iris::Vector(-4.0, 8.0, -4.0),
            iris::CrossProduct(operand0, operand1));
}

TEST(VectorTest, Normalize) {
  auto vector = iris::Vector(2.0, 0.0, 0.0);
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0), iris::Normalize(vector));

  iris::geometric length_squared, length;
  EXPECT_EQ(iris::Vector(1.0, 0.0, 0.0),
            iris::Normalize(vector, &length_squared, &length));
  EXPECT_EQ(4.0, length_squared);
  EXPECT_EQ(2.0, length);
}

TEST(VectorTest, DiminishedAxis) {
  EXPECT_EQ(iris::Vector::X_AXIS,
            iris::Vector(1.0, -2.0, 3.0).DiminishedAxis());
  EXPECT_EQ(iris::Vector::Y_AXIS,
            iris::Vector(-2.0, 1.0, 3.0).DiminishedAxis());
  EXPECT_EQ(iris::Vector::Z_AXIS,
            iris::Vector(3.0, -2.0, 1.0).DiminishedAxis());
}

TEST(VectorTest, DominantAxis) {
  EXPECT_EQ(iris::Vector::X_AXIS, iris::Vector(3.0, -2.0, 1.0).DominantAxis());
  EXPECT_EQ(iris::Vector::Y_AXIS, iris::Vector(-2.0, 3.0, 1.0).DominantAxis());
  EXPECT_EQ(iris::Vector::Z_AXIS, iris::Vector(1.0, -2.0, 3.0).DominantAxis());
}