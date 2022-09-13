#ifndef _IRIS_CAMERA_
#define _IRIS_CAMERA_

#include <array>

#include "iris/float.h"
#include "iris/ray.h"

namespace iris {

class Camera {
 public:
  virtual Ray Compute(const std::array<geometric_t, 2>& image_uv,
                      const std::array<geometric_t, 2>* lens_uv) const = 0;
  virtual bool HasLens() const = 0;
  virtual ~Camera() {}
};

}  // namespace iris

#endif  // _IRIS_CAMERA_