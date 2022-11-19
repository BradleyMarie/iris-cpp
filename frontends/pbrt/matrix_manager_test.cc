#include "frontends/pbrt/matrix_manager.h"

#include <memory>

#include "googletest/include/gtest/gtest.h"

TEST(MatrixManager, InitialState) {
  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().start);
  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().end);
}

TEST(MatrixManager, Empty) {
  std::stringstream input("");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_FALSE(matrix_manager.TryParse(tokenizer));
}

TEST(MatrixManager, WrongDirective) {
  std::stringstream input("Shape");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_FALSE(matrix_manager.TryParse(tokenizer));
}

TEST(MatrixManager, MissingPop) {
  std::function<void()> body = []() {
    std::stringstream input("Shape");
    iris::pbrt_frontend::Tokenizer tokenizer(input);
    auto matrix_manager =
        std::make_unique<iris::pbrt_frontend::MatrixManager>();
    matrix_manager->Push();
    matrix_manager.reset();
  };

  EXPECT_EXIT(body(), testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Missing AttributeEnd directive");
}

TEST(MatrixManager, BadPop) {
  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EXIT(matrix_manager.Pop(), testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Missing AttributeBegin directive");
}

TEST(MatrixManager, TooFewParameters) {
  std::stringstream input("Translate 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few parameters to directive Translate");
}

TEST(MatrixManager, FailedToParse) {
  std::stringstream input("Translate 1.0 Scale");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Failed to parse Translate parameter: Scale");
}

TEST(MatrixManager, OutOfRange) {
  std::stringstream input("Translate 1.0 1.0 Inf");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Out of range Translate parameter: Inf");
}

TEST(MatrixManager, ActiveTransformBad) {
  std::stringstream input("ActiveTransform Translate");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid parameter to ActiveTransform: Translate");
}

TEST(MatrixManager, ActiveTransformStartTime) {
  std::stringstream input("ActiveTransform StartTime Translate 1.0 1.0 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));

  EXPECT_EQ(iris::Matrix::Translation(1.0, 1.0, 1.0).value(),
            matrix_manager.Get().start);
  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().end);
}

TEST(MatrixManager, ActiveTransformStartEnd) {
  std::stringstream input("ActiveTransform EndTime Translate 1.0 1.0 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));

  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().start);
  EXPECT_EQ(iris::Matrix::Translation(1.0, 1.0, 1.0).value(),
            matrix_manager.Get().end);
}

TEST(MatrixManager, ActiveTransformStartAll) {
  std::stringstream input("ActiveTransform All Translate 1.0 1.0 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));

  EXPECT_EQ(iris::Matrix::Translation(1.0, 1.0, 1.0).value(),
            matrix_manager.Get().start);
  EXPECT_EQ(iris::Matrix::Translation(1.0, 1.0, 1.0).value(),
            matrix_manager.Get().end);
}

TEST(MatrixManager, Identity) {
  std::stringstream input("Translate 1.0 1.0 1.0 Identity");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));

  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().start);
}

TEST(MatrixManager, Translate) {
  std::stringstream input("Translate 1.0 1.0 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));

  EXPECT_EQ(iris::Matrix::Translation(1.0, 1.0, 1.0).value(),
            matrix_manager.Get().start);
}

TEST(MatrixManager, ScaleX) {
  std::stringstream input("Scale 0.0 1.0 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Parameters to Scale must be non-zero");
}

TEST(MatrixManager, ScaleY) {
  std::stringstream input("Scale 1.0 0.0 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Parameters to Scale must be non-zero");
}

TEST(MatrixManager, ScaleZ) {
  std::stringstream input("Scale 1.0 1.0 0.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Parameters to Scale must be non-zero");
}

TEST(MatrixManager, Scale) {
  std::stringstream input("Scale 2.0 2.0 2.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));

  EXPECT_EQ(iris::Matrix::Scalar(2.0, 2.0, 2.0).value(),
            matrix_manager.Get().start);
}

TEST(MatrixManager, RotateX) {
  std::stringstream input("Rotate 1.0 0.0 0.0 0.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_EXIT(
      matrix_manager.TryParse(tokenizer), testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: One of the x, y, or z parameters to Rotate must be non-zero");
}

TEST(MatrixManager, Rotate) {
  std::stringstream input("Rotate 1.0 2.0 2.0 2.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));

  EXPECT_EQ(iris::Matrix::Rotation(1.0, 2.0, 2.0, 2.0).value(),
            matrix_manager.Get().start);
}

TEST(MatrixManager, LookAtSamePoint) {
  std::stringstream input("LookAt 1.0 1.0 1.0 1.0 1.0 1.0 0.0 0.0 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: eye and look_at parameters to LookAt must not be equal");
}

TEST(MatrixManager, LookAtNoUp) {
  std::stringstream input("LookAt 1.0 2.0 3.0 1.0 1.0 1.0 0.0 0.0 0.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: One of up_x, up_y, or up_z parameters to LookAt must be "
              "non-zero");
}

TEST(MatrixManager, LookAtUpParallel) {
  std::stringstream input("LookAt 0.0 0.0 0.0 0.0 0.0 1.0 0.0 0.0 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(
      matrix_manager.TryParse(tokenizer), testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Up parameter to LookAt must not be parallel with look direction");
}

TEST(MatrixManager, LookAt) {
  std::stringstream input("LookAt 0.0 0.0 0.0 0.0 0.0 1.0 0.0 1.0 1.0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EQ(
      iris::Matrix::LookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0).value(),
      matrix_manager.Get().start);
}

TEST(MatrixManager, TransformNotInvertible) {
  std::stringstream input("Transform 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Uninvertible matrix specified for Transform directive");
}

TEST(MatrixManager, Transform) {
  std::stringstream input("Transform 2 0 0 0 0 2 0 0 0 0 2 0 0 0 0 1");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));

  EXPECT_EQ(iris::Matrix::Scalar(2.0, 2.0, 2.0).value(),
            matrix_manager.Get().start);
}

TEST(MatrixManager, ConcatTransformNotInvertible) {
  std::stringstream input("ConcatTransform 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(
      matrix_manager.TryParse(tokenizer), testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Uninvertible matrix specified for ConcatTransform directive");
}

TEST(MatrixManager, ConcatTransform) {
  std::stringstream input("ConcatTransform 2 0 0 0 0 2 0 0 0 0 2 0 0 0 0 1");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;
  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));

  EXPECT_EQ(iris::Matrix::Scalar(2.0, 2.0, 2.0).value(),
            matrix_manager.Get().start);
}

TEST(MatrixManager, CoordinateSystemBad) {
  std::stringstream input("CoordinateSystem");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few parameters to directive CoordinateSystem");
}

TEST(MatrixManager, CoordinateSystemNotQuoted) {
  std::stringstream input("CoordinateSystem Scalar");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Invalid parameter to directive CoordinateSystem: Scalar");
}

TEST(MatrixManager, CoordSysTransformBad) {
  std::stringstream input("CoordSysTransform");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Too few parameters to directive CoordSysTransform");
}

TEST(MatrixManager, CoordSysTransformNotQuoted) {
  std::stringstream input("CoordSysTransform Scalar");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(
      matrix_manager.TryParse(tokenizer), testing::ExitedWithCode(EXIT_FAILURE),
      "ERROR: Invalid parameter to directive CoordSysTransform: Scalar");
}

TEST(MatrixManager, CoordSysTransformNotFound) {
  std::stringstream input("CoordSysTransform \"scalar\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_EXIT(matrix_manager.TryParse(tokenizer),
              testing::ExitedWithCode(EXIT_FAILURE),
              "Unknown coordinate system name specified for directive "
              "CoordSysTransform: \"scalar\"");
}

TEST(MatrixManager, CoordSysTransform) {
  std::stringstream input(
      "Translate 1 2 3 CoordinateSystem \"a\" Identity CoordSysTransform "
      "\"a\"");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_EQ(iris::Matrix::Translation(1.0, 2.0, 3.0).value(),
            matrix_manager.Get().start);

  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_EQ(iris::Matrix::Translation(1.0, 2.0, 3.0).value(),
            matrix_manager.Get().start);

  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().start);

  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_EQ(iris::Matrix::Translation(1.0, 2.0, 3.0).value(),
            matrix_manager.Get().start);
}

TEST(MatrixManager, MismatchedPushAndPop) {
  std::function<void()> body0 = []() {
    std::stringstream input("TransformBegin");
    iris::pbrt_frontend::Tokenizer tokenizer(input);

    iris::pbrt_frontend::MatrixManager matrix_manager;

    EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
    matrix_manager.Pop();
  };

  EXPECT_EXIT(body0(), testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Mismatched TransformBegin and TransformEnd directives");

  std::function<void()> body1 = []() {
    std::stringstream input("TransformEnd");
    iris::pbrt_frontend::Tokenizer tokenizer(input);

    iris::pbrt_frontend::MatrixManager matrix_manager;

    matrix_manager.Push();
    EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  };

  EXPECT_EXIT(body1(), testing::ExitedWithCode(EXIT_FAILURE),
              "ERROR: Mismatched AttributeBegin and AttributeEnd directives");
}

TEST(MatrixManager, TransformBeginTransformEnd) {
  std::stringstream input("TransformBegin Translate 1 2 3 TransformEnd");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().start);

  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_EQ(iris::Matrix::Translation(1.0, 2.0, 3.0).value(),
            matrix_manager.Get().start);

  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().start);
}

TEST(MatrixManager, PushPop) {
  std::stringstream input("Translate 1 2 3");
  iris::pbrt_frontend::Tokenizer tokenizer(input);

  iris::pbrt_frontend::MatrixManager matrix_manager;

  matrix_manager.Push();
  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().start);

  EXPECT_TRUE(matrix_manager.TryParse(tokenizer));
  EXPECT_EQ(iris::Matrix::Translation(1.0, 2.0, 3.0).value(),
            matrix_manager.Get().start);

  matrix_manager.Pop();
  EXPECT_EQ(iris::Matrix::Identity(), matrix_manager.Get().start);
}