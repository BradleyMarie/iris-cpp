#include "frontends/pbrt/matrix_manager.h"

#include <array>
#include <cassert>
#include <charconv>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include "frontends/pbrt/quoted_string.h"

namespace iris::pbrt_frontend {
namespace {

template <size_t N>
std::array<geometric, N> ParseFloats(Tokenizer& tokenizer,
                                     std::string_view type_name) {
  std::array<geometric, N> result;
  for (size_t i = 0; i < N; i++) {
    auto token = tokenizer.Next();
    if (!token) {
      std::cerr << "ERROR: Too few parameters to directive " << type_name
                << std::endl;
      exit(EXIT_FAILURE);
    }

    if (std::from_chars(token->begin(), token->end(), result[i]).ec !=
        std::errc{}) {
      std::cerr << "ERROR: Failed to parse " << type_name
                << " parameter: " << *token << std::endl;
      exit(EXIT_FAILURE);
    }

    if (!std::isfinite(result[i])) {
      std::cerr << "ERROR: Out of range " << type_name
                << " parameter: " << *token << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  return result;
}

};  // namespace

MatrixManager::MatrixManager() {
  transformation_stack_.emplace(Matrix::Identity(), Matrix::Identity());
  active_transform_stack_.emplace(ActiveTransformation::ALL);
}

MatrixManager::~MatrixManager() {
  if (!stack_manipulation_reasons_.empty()) {
    const char* message =
        (stack_manipulation_reasons_.top() == PushPopReason::ATTRIBUTE)
            ? "AttributeEnd"
            : "TransformEnd";
    std::cerr << "ERROR: Missing " << message << " directive" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void MatrixManager::Identity(Tokenizer& tokenizer) { Set(Matrix::Identity()); }

void MatrixManager::Translate(Tokenizer& tokenizer) {
  auto values = ParseFloats<3>(tokenizer, "Translate");
  Transform(Matrix::Translation(values[0], values[1], values[2]).value());
}

void MatrixManager::Scale(Tokenizer& tokenizer) {
  auto values = ParseFloats<3>(tokenizer, "Scale");
  if (values[0] == 0.0 || values[1] == 0.0 || values[2] == 0.0) {
    std::cerr << "ERROR: Parameters to Scale must be non-zero" << std::endl;
    exit(EXIT_FAILURE);
  }

  Transform(Matrix::Scalar(values[0], values[1], values[2]).value());
}

void MatrixManager::Rotate(Tokenizer& tokenizer) {
  auto values = ParseFloats<4>(tokenizer, "Rotate");
  if (values[1] == 0.0 && values[2] == 0.0 && values[3] == 0.0) {
    std::cerr
        << "ERROR: One of the x, y, or z parameters to Rotate must be non-zero"
        << std::endl;
    exit(EXIT_FAILURE);
  }

  Transform(
      Matrix::Rotation(values[0], values[1], values[2], values[3]).value());
}

void MatrixManager::LookAt(Tokenizer& tokenizer) {
  auto values = ParseFloats<9>(tokenizer, "LookAt");
  if (values[0] == values[3] && values[1] == values[4] &&
      values[2] == values[5]) {
    std::cerr << "ERROR: eye and look_at parameters to LookAt must not be equal"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (values[6] == 0.0 && values[7] == 0.0 && values[8] == 0.0) {
    std::cerr << "ERROR: One of up_x, up_y, or up_z parameters to LookAt must "
                 "be non-zero"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto look_at =
      Matrix::LookAt(values[0], values[1], values[2], values[3], values[4],
                     values[5], values[6], values[7], values[8]);
  if (!look_at.ok()) {
    assert(look_at.status().message() == "up");
    std::cerr << "ERROR: Up parameter to LookAt must not be parallel with look "
                 "direction"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  Transform(look_at.value());
}

void MatrixManager::CoordinateSystem(Tokenizer& tokenizer) {
  auto next = tokenizer.Next();
  if (!next) {
    std::cerr << "ERROR: Too few parameters to directive CoordinateSystem"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (!Unquote(*next)) {
    std::cerr << "ERROR: Invalid parameter to directive CoordinateSystem: "
              << *next << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string key(next->begin(), next->end());

  coord_systems_.erase(key);
  coord_systems_.emplace(std::move(key),
                         std::make_pair(transformation_stack_.top().start,
                                        transformation_stack_.top().end));
}

void MatrixManager::CoordSysTransform(Tokenizer& tokenizer) {
  auto next = tokenizer.Next();
  if (!next) {
    std::cerr << "ERROR: Too few parameters to directive CoordSysTransform"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (!Unquote(*next)) {
    std::cerr << "ERROR: Invalid parameter to directive CoordSysTransform: "
              << *next << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string key(next->begin(), next->end());

  auto iter = coord_systems_.find(key);
  if (iter == coord_systems_.end()) {
    std::cerr << "ERROR: Unknown coordinate system name specified for "
                 "directive CoordSysTransform: "
              << *next << std::endl;
    exit(EXIT_FAILURE);
  }

  transformation_stack_.pop();
  transformation_stack_.emplace(iter->second.first, iter->second.second);
}

void MatrixManager::Transform(Tokenizer& tokenizer) {
  auto values = ParseFloats<16>(tokenizer, "Transform");
  std::array<std::array<geometric, 4>, 4> values2d = {
      std::array<geometric, 4>{values[0], values[1], values[2], values[3]},
      std::array<geometric, 4>{values[4], values[5], values[6], values[7]},
      std::array<geometric, 4>{values[8], values[9], values[10], values[11]},
      std::array<geometric, 4>{values[12], values[13], values[14], values[15]}};

  auto matrix = Matrix::Create(values2d);
  if (!matrix.ok()) {
    std::cerr << "ERROR: Uninvertible matrix specified for Transform directive"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  Set(matrix.value());
}

void MatrixManager::ConcatTransform(Tokenizer& tokenizer) {
  auto values = ParseFloats<16>(tokenizer, "ConcatTransform");
  std::array<std::array<geometric, 4>, 4> values2d = {
      std::array<geometric, 4>{values[0], values[1], values[2], values[3]},
      std::array<geometric, 4>{values[4], values[5], values[6], values[7]},
      std::array<geometric, 4>{values[8], values[9], values[10], values[11]},
      std::array<geometric, 4>{values[12], values[13], values[14], values[15]}};

  auto matrix = Matrix::Create(values2d);
  if (!matrix.ok()) {
    std::cerr
        << "ERROR: Uninvertible matrix specified for ConcatTransform directive"
        << std::endl;
    exit(EXIT_FAILURE);
  }

  Transform(matrix.value());
}

void MatrixManager::ActiveTransform(Tokenizer& tokenizer) {
  auto next = tokenizer.Next();
  if (!next) {
    std::cerr << "ERROR: Too few parameters to directive ActiveTransform"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (*next == "StartTime") {
    active_transform_stack_.top() = ActiveTransformation::START;
  } else if (*next == "EndTime") {
    active_transform_stack_.top() = ActiveTransformation::END;
  } else if (*next == "All") {
    active_transform_stack_.top() = ActiveTransformation::ALL;
  } else {
    std::cerr << "ERROR: Invalid parameter to ActiveTransform: " << *next
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

void MatrixManager::TransformBegin(Tokenizer& tokenizer) {
  Push(PushPopReason::TRANSFORM);
}

void MatrixManager::TransformEnd(Tokenizer& tokenizer) {
  Pop(PushPopReason::TRANSFORM);
}

void MatrixManager::Transform(const Matrix& m) {
  Matrix start = (active_transform_stack_.top() != ActiveTransformation::END)
                     ? transformation_stack_.top().start.Multiply(m)
                     : transformation_stack_.top().start;

  Matrix end = (active_transform_stack_.top() != ActiveTransformation::START)
                   ? transformation_stack_.top().end.Multiply(m)
                   : transformation_stack_.top().end;

  transformation_stack_.pop();
  transformation_stack_.emplace(start, end);
}

void MatrixManager::Set(const Matrix& m) {
  Matrix start = (active_transform_stack_.top() != ActiveTransformation::END)
                     ? m
                     : transformation_stack_.top().start;

  Matrix end = (active_transform_stack_.top() != ActiveTransformation::START)
                   ? m
                   : transformation_stack_.top().end;

  transformation_stack_.pop();
  transformation_stack_.emplace(start, end);
}

bool MatrixManager::TryParse(Tokenizer& tokenizer) {
  auto next = tokenizer.Peek();
  if (!next) {
    return false;
  }

  static const std::unordered_map<absl::string_view,
                                  void (MatrixManager::*)(Tokenizer&)>
      callbacks = {
          {"ActiveTransform", &MatrixManager::ActiveTransform},
          {"ConcatTransform", &MatrixManager::ConcatTransform},
          {"CoordinateSystem", &MatrixManager::CoordinateSystem},
          {"CoordSysTransform", &MatrixManager::CoordSysTransform},
          {"Identity", &MatrixManager::Identity},
          {"LookAt", &MatrixManager::LookAt},
          {"Rotate", &MatrixManager::Rotate},
          {"Scale", &MatrixManager::Scale},
          {"Transform", &MatrixManager::Transform},
          {"TransformBegin", &MatrixManager::TransformBegin},
          {"TransformEnd", &MatrixManager::TransformEnd},
          {"Translate", &MatrixManager::Translate},
      };

  auto iter = callbacks.find(*next);
  if (iter == callbacks.end()) {
    return false;
  }

  // Consume peeked token
  tokenizer.Next();

  (this->*iter->second)(tokenizer);

  return true;
}

const MatrixManager::Transformation& MatrixManager::Get() {
  return transformation_stack_.top();
}

void MatrixManager::Push() { Push(PushPopReason::ATTRIBUTE); }

void MatrixManager::Pop() { Pop(PushPopReason::ATTRIBUTE); }

void MatrixManager::Push(PushPopReason reason) {
  transformation_stack_.emplace(transformation_stack_.top());
  stack_manipulation_reasons_.push(reason);
}

void MatrixManager::Pop(PushPopReason reason) {
  if (stack_manipulation_reasons_.empty()) {
    const char* message = (reason == PushPopReason::ATTRIBUTE)
                              ? "AttributeBegin"
                              : "TransformBegin";
    std::cerr << "ERROR: Missing " << message << " directive" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (stack_manipulation_reasons_.top() != reason) {
    const char* message =
        (stack_manipulation_reasons_.top() == PushPopReason::ATTRIBUTE)
            ? "AttributeBegin and AttributeEnd"
            : "TransformBegin and TransformEnd";
    std::cerr << "ERROR: Mismatched " << message << " directives" << std::endl;
    exit(EXIT_FAILURE);
  }

  transformation_stack_.pop();
  stack_manipulation_reasons_.pop();
}

}  // namespace iris::pbrt_frontend