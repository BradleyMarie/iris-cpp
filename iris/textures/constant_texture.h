#ifndef _IRIS_TEXTURES_CONSTANT_TEXTURE_
#define _IRIS_TEXTURES_CONSTANT_TEXTURE_

#include "iris/reference_counted.h"
#include "iris/texture_coordinates.h"
#include "iris/textures/texture2d.h"

namespace iris {
namespace textures {

template <typename Return, typename... Args>
class ConstantPointerTexture2D final
    : public PointerTexture2D<Return, Args...> {
 public:
  ConstantPointerTexture2D(iris::ReferenceCounted<Return> value)
      : value_(std::move(value)) {}

  const Return* Evaluate(const TextureCoordinates& coordinates,
                         Args&... args) const override {
    return value_.Get();
  }

 private:
  const iris::ReferenceCounted<Return> value_;
};

template <typename T>
class ConstantValueTexture2D final : public ValueTexture2D<T> {
 public:
  ConstantValueTexture2D(const T& value) : value_(value) {}

  T Evaluate(const TextureCoordinates& coordinates) const override {
    return value_;
  }

 private:
  T value_;
};

}  // namespace textures
}  // namespace iris

#endif  // _IRIS_TEXTURES_TEXTURE_2D_