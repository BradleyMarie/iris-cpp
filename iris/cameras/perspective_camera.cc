#include "iris/cameras/perspective_camera.h"

#define _USE_MATH_CONSTANTS
#include <cassert>
#include <cmath>

namespace iris {
namespace cameras {
namespace {

Point ComputeFrameStart(const std::array<geometric_t, 4>& frame_bounds,
                        geometric_t fov) {
  intermediate_t half_fov_radians =
      fov * static_cast<intermediate_t>(M_PI / 360.0);
  geometric_t image_plane_distance = 1.0 / std::tan(half_fov_radians);
  return Point(frame_bounds[0], frame_bounds[3], image_plane_distance);
}

}  // namespace

PerspectiveCamera::PerspectiveCamera(
    const Matrix& camera_to_world,
    const std::array<geometric_t, 4>& frame_bounds, geometric_t fov) noexcept
    : camera_to_world_(camera_to_world),
      frame_start_(ComputeFrameStart(frame_bounds, fov)),
      frame_size_({frame_bounds[2] - frame_bounds[0],
                   frame_bounds[1] - frame_bounds[3]}) {
  assert(std::isfinite(frame_bounds[0]));
  assert(std::isfinite(frame_bounds[1]));
  assert(std::isfinite(frame_bounds[2]));
  assert(std::isfinite(frame_bounds[3]));
  assert(frame_bounds[0] < frame_bounds[2]);
  assert(frame_bounds[1] < frame_bounds[3]);
  assert(0.0 < fov && fov < 180.0);
}

Ray PerspectiveCamera::Compute(
    const std::array<geometric_t, 2>& image_uv,
    const std::array<geometric_t, 2>* lens_uv) const {
  Point camera_origin(0.0, 0.0, 0.0);
  Point image_plane(frame_start_.x + frame_size_[0] * image_uv[0],
                    frame_start_.y + frame_size_[1] * image_uv[1],
                    frame_start_.z);
  Ray camera_ray(camera_origin, image_plane - camera_origin);
  return Normalize(camera_to_world_.Multiply(camera_ray));
}

bool PerspectiveCamera::HasLens() const { return false; }

}  // namespace cameras
}  // namespace iris