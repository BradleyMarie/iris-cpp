#ifndef _IRIS_CAMERA_ORTHOGRAPHIC_CAMERA_
#define _IRIS_CAMERA_ORTHOGRAPHIC_CAMERA_

#include <array>

#include "iris/camera.h"
#include "iris/float.h"
#include "iris/matrix.h"
#include "iris/ray.h"

namespace iris {
namespace cameras {

class OrthographicCamera final : public Camera {
 public:
  OrthographicCamera(const Matrix& camera_to_world,
                     const std::array<geometric_t, 4>& frame_bounds) noexcept;

  Ray Compute(const std::array<geometric_t, 2>& image_uv,
              const std::array<geometric_t, 2>* lens_uv) const override;
  bool HasLens() const override;

 private:
  const Matrix camera_to_world_;
  const std::array<geometric_t, 2> frame_start_;
  const std::array<geometric_t, 2> frame_size_;
};

}  // namespace cameras
}  // namespace iris

#endif  // _IRIS_CAMERA_ORTHOGRAPHIC_CAMERA_