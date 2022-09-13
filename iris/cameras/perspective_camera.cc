#include "iris/cameras/perspective_camera.h"

#include <cassert>
#include <cmath>

namespace iris {
namespace cameras {
namespace {

std::tuple<Vector, Vector, Vector> ComputeImagePlane(geometric_t aspect_ratio,
                                                     geometric_t fov) {
  geometric_t pi = std::acos(-1.0);
  geometric_t half_fov_radians = fov * pi / 360.0;
  geometric_t image_dimension = 2.0 * std::tan(half_fov_radians);

  geometric_t x_dim, y_dim;
  if (aspect_ratio < 1.0) {
    x_dim = image_dimension;
    y_dim = image_dimension / aspect_ratio;
  } else {
    x_dim = image_dimension * aspect_ratio;
    y_dim = image_dimension;
  }

  Vector image_plane_u(x_dim, 0.0, 0.0);
  Vector image_plane_v(0.0, -y_dim, 0.0);
  Vector to_image_plane =
      Vector(0.0, 0.0, 1.0) - image_plane_u * 0.5 - image_plane_v * 0.5;

  return std::make_tuple(to_image_plane, image_plane_u, image_plane_v);
}

}  // namespace

PerspectiveCamera::PerspectiveCamera(const Matrix& camera_to_world,
                                     geometric_t aspect_ratio, geometric_t fov)
    : camera_to_world_(camera_to_world),
      image_plane_(ComputeImagePlane(aspect_ratio, fov)) {
  assert(std::isfinite(aspect_ratio) && aspect_ratio > 0.0);
  assert(0.0 < fov && fov < 180.0);
}

Ray PerspectiveCamera::Compute(
    const std::array<geometric_t, 2>& image_uv,
    const std::array<geometric_t, 2>* lens_uv) const {
  Point camera_origin(0.0, 0.0, 0.0);
  Vector camera_direction = std::get<0>(image_plane_) +
                            std::get<1>(image_plane_) * image_uv[0] +
                            std::get<2>(image_plane_) * image_uv[1];
  Ray camera_ray(camera_origin, camera_direction);
  return Normalize(camera_to_world_.Multiply(camera_ray));
}

bool PerspectiveCamera::HasLens() const { return false; }

}  // namespace cameras
}  // namespace iris