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