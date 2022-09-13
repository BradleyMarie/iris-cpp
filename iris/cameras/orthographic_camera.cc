#include "iris/cameras/orthographic_camera.h"

namespace iris {
namespace cameras {

Ray OrthographicCamera::Compute(
    const std::array<geometric_t, 2>& image_uv,
    const std::array<geometric_t, 2>* lens_uv) const {
  Point camera_origin(-0.5 + image_uv[0], 0.5 - image_uv[1], 0.0);
  Ray camera_ray(camera_origin, Vector(0.0, 0.0, 1.0));
  return Normalize(camera_to_world_.Multiply(camera_ray));
}

bool OrthographicCamera::HasLens() const { return false; }

}  // namespace cameras
}  // namespace iris