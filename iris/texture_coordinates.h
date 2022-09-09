#ifndef _IRIS_TEXTURE_COORDINATES_
#define _IRIS_TEXTURE_COORDINATES_

#include <array>
#include <optional>

#include "iris/float.h"
#include "iris/vector.h"

namespace iris {

struct TextureCoordinates final {
  struct Derivatives {
    geometric du_dx;
    geometric du_dy;
    geometric dv_dx;
    geometric dv_dy;
    Vector dp_du;
    Vector dp_dv;
  };

  std::array<geometric, 2> uv;
  std::optional<Derivatives> derivatives;
};

}  // namespace iris

#endif  // _IRIS_TEXTURE_COORDINATES_