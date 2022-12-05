#ifndef _IRIS_CAMERA_PERSPECTIVE_CAMERA_
#define _IRIS_CAMERA_PERSPECTIVE_CAMERA_

#include <array>
#include <tuple>

#include "iris/camera.h"
#include "iris/float.h"
#include "iris/matrix.h"
#include "iris/ray.h"
#include "iris/vector.h"

namespace iris {
namespace cameras {

class PerspectiveCamera final : public Camera {
 public:
  // fov is specified in degrees
  PerspectiveCamera(const Matrix& camera_to_world,
                    const std::array<geometric_t, 4>& frame_bounds,
                    geometric_t fov) noexcept;

  Ray Compute(const std::array<geometric_t, 2>& image_uv,
              const std::array<geometric_t, 2>* lens_uv) const override;
  bool HasLens() const override;

 private:
  const Matrix camera_to_world_;
  const Point frame_start_;
  const std::array<geometric_t, 2> frame_size_;
};

}  // namespace cameras
}  // namespace iris

#endif  // _IRIS_CAMERA_PERSPECTIVE_CAMERA_