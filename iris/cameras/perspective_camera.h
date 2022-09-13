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
  PerspectiveCamera(const Matrix& camera_to_world, geometric_t aspect_ratio,
                    geometric_t fov);

  Ray Compute(const std::array<geometric_t, 2>& image_uv,
              const std::array<geometric_t, 2>* lens_uv) const override;
  bool HasLens() const override;

 private:
  Matrix camera_to_world_;
  std::tuple<Vector, Vector, Vector> image_plane_;
};

}  // namespace cameras
}  // namespace iris

#endif  // _IRIS_CAMERA_PERSPECTIVE_CAMERA_