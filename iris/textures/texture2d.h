#ifndef _IRIS_TEXTURES_TEXTURE_2D_
#define _IRIS_TEXTURES_TEXTURE_2D_

#include "iris/reference_countable.h"
#include "iris/texture_coordinates.h"

namespace iris {
namespace textures {

template <typename T, typename... Args>
class PointerTexture2D : public ReferenceCountable {
 public:
  virtual const T* Evaluate(const TextureCoordinates& coordinates,
                            Args&... args) const = 0;
  virtual ~PointerTexture2D() = default;
};

template <typename T>
class ValueTexture2D : public ReferenceCountable {
 public:
  virtual T Evaluate(const TextureCoordinates& coordinates) const = 0;
  virtual ~ValueTexture2D() = default;
};

}  // namespace textures
}  // namespace iris

#endif  // _IRIS_TEXTURES_TEXTURE_2D_