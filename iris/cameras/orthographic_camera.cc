#include "iris/cameras/orthographic_camera.h"

#include <cassert>
#include <cmath>

namespace iris {
namespace cameras {

OrthographicCamera::OrthographicCamera(
    const Matrix& camera_to_world,
    const std::array<geometric_t, 4>& frame_bounds) noexcept
    : camera_to_world_(camera_to_world),
      frame_start_({frame_bounds[0], frame_bounds[3]}),
      frame_size_({frame_bounds[2] - frame_bounds[0],
                   frame_bounds[1] - frame_bounds[3]}) {
  assert(std::isfinite(frame_bounds[0]));
  assert(std::isfinite(frame_bounds[1]));
  assert(std::isfinite(frame_bounds[2]));
  assert(std::isfinite(frame_bounds[3]));
  assert(frame_bounds[0] < frame_bounds[2]);
  assert(frame_bounds[1] < frame_bounds[3]);
}

Ray OrthographicCamera::Compute(
    const std::array<geometric_t, 2>& image_uv,
    const std::array<geometric_t, 2>* lens_uv) const {
  Point camera_origin(frame_start_[0] + frame_size_[0] * image_uv[0],
                      frame_start_[1] + frame_size_[1] * image_uv[1], 0.0);
  Ray camera_ray(camera_origin, Vector(0.0, 0.0, 1.0));
  return Normalize(camera_to_world_.Multiply(camera_ray));
}

bool OrthographicCamera::HasLens() const { return false; }

}  // namespace cameras
}  // namespace iris