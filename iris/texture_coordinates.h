#ifndef _IRIS_TEXTURE_COORDINATES_
#define _IRIS_TEXTURE_COORDINATES_

#include <array>
#include <optional>

#include "iris/float.h"
#include "iris/vector.h"

namespace iris {

struct TextureCoordinates final {
  struct Derivatives {
    geometric_t du_dx;
    geometric_t du_dy;
    geometric_t dv_dx;
    geometric_t dv_dy;
    Vector dp_du;
    Vector dp_dv;
  };

  const std::array<geometric_t, 2> uv;
  const std::optional<Derivatives> derivatives;
};

}  // namespace iris

#endif  // _IRIS_TEXTURE_COORDINATES_