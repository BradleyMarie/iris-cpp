#ifndef _FRONTENDS_PBRT_MATRIX_MANAGER_
#define _FRONTENDS_PBRT_MATRIX_MANAGER_

#include <stack>
#include <string>
#include <unordered_map>
#include <utility>

#include "frontends/pbrt/tokenizer.h"
#include "iris/matrix.h"

namespace iris::pbrt_frontend {

class MatrixManager {
 public:
  MatrixManager();
  ~MatrixManager();

  bool TryParse(Tokenizer& tokenizer);

  struct Transformation {
    Matrix start;
    Matrix end;
  };

  const Transformation& Get();

  void Push();
  void Pop();

 private:
  enum class ActiveTransformation { START, END, ALL };
  enum class PushPopReason { ATTRIBUTE, TRANSFORM };

  void Push(PushPopReason reason);
  void Pop(PushPopReason reason);

  void Identity(Tokenizer& tokenizer);
  void Translate(Tokenizer& tokenizer);
  void Scale(Tokenizer& tokenizer);
  void Rotate(Tokenizer& tokenizer);
  void LookAt(Tokenizer& tokenizer);
  void CoordinateSystem(Tokenizer& tokenizer);
  void CoordSysTransform(Tokenizer& tokenizer);
  void Transform(Tokenizer& tokenizer);
  void ConcatTransform(Tokenizer& tokenizer);
  void ActiveTransform(Tokenizer& tokenizer);
  void TransformBegin(Tokenizer& tokenizer);
  void TransformEnd(Tokenizer& tokenizer);

  void Transform(const Matrix& m);
  void Set(const Matrix& m);

  std::stack<Transformation> transformation_stack_;
  std::stack<ActiveTransformation> active_transform_stack_;
  std::stack<PushPopReason> stack_manipulation_reasons_;
  std::unordered_map<std::string, std::pair<Matrix, Matrix>> coord_systems_;
};

}  // namespace iris::pbrt_frontend

#endif  // _FRONTENDS_PBRT_MATRIX_MANAGER_