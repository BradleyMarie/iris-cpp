#ifndef _IRIS_COLOR_
#define _IRIS_COLOR_

#include <cassert>

#include "iris/float.h"

namespace iris {

struct Color final {
  enum Space {
    CIE_XYZ,
    LINEAR_SRGB,
  };

  explicit Color(geometric_t c0, geometric_t c1, geometric_t c2, Space space)
      : x(c0), y(c1), z(c2), space(space) {
    assert(x >= 0.0);
    assert(y >= 0.0);
    assert(z >= 0.0);
  }

  Color(const Color&) = default;

  bool operator==(const Color&) const = default;

  const geometric_t& operator[](size_t index) const {
    assert(index < 3);
    const geometric_t* as_array = &x;
    return as_array[index];
  }

  union {
    struct {
      const visual_t x;
      const visual_t y;
      const visual_t z;
    };
    struct {
      const visual_t r;
      const visual_t g;
      const visual_t b;
    };
  };

  Space space;
};

}  // namespace iris

#endif  // _IRIS_COLOR_