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

  explicit Color(geometric c0, geometric c1, geometric c2, Space space)
      : x(c0), y(c1), z(c2), space(space) {
    assert(x >= 0.0);
    assert(y >= 0.0);
    assert(z >= 0.0);
  }

  Color(const Color&) = default;

  bool operator==(const Color&) const = default;

  const geometric& operator[](size_t index) const {
    assert(index < 3);
    const geometric* as_array = &x;
    return as_array[index];
  }

  union {
    struct {
      const visual x;
      const visual y;
      const visual z;
    };
    struct {
      const visual r;
      const visual g;
      const visual b;
    };
  };

  Space space;
};

}  // namespace iris

#endif  // _IRIS_COLOR_