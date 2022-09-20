#include "iris/bsdfs/lambertian_brdf.h"

#define _USE_MATH_CONSTANTS
#include <cmath>
#include <random>

#include "iris/bsdfs/utility/local.h"

namespace iris {
namespace bsdfs {
namespace {

Vector SampleHemisphereWithCosineWeighting(const Vector& surface_normal,
                                           Random& rng) {
  std::uniform_real_distribution<geometric> distribution;

  auto radius_squared = distribution(rng);
  auto radius = std::sqrt(radius_squared);

  auto theta = distribution(rng) * (2.0 * M_PI) - M_PI;
  auto sin_theta = std::sin(theta);
  auto cos_theta = std::cos(theta);

  float_t x = radius * cos_theta;
  float_t y = radius * sin_theta;

  auto coordinate_system = utility::CreateLocalCoordinateSpace(surface_normal);

  return utility::Transform(Vector(x, y, std::sqrt(1.0 - radius_squared)),
                            surface_normal, coordinate_system.first,
                            coordinate_system.second);
}

}  // namespace

std::optional<Bsdf::Sample> LambertianBrdf::SampleAll(
    const Vector& incoming, const Vector& surface_normal,
    const Vector& shading_normal, Random& rng,
    SpectralAllocator& allocator) const {
  return SampleDiffuse(incoming, surface_normal, shading_normal, rng,
                       allocator);
}

std::optional<Bsdf::Sample> LambertianBrdf::SampleDiffuse(
    const Vector& incoming, const Vector& surface_normal,
    const Vector& shading_normal, Random& rng,
    SpectralAllocator& allocator) const {
  Vector sampled_direction =
      SampleHemisphereWithCosineWeighting(shading_normal, rng);
  if (DotProduct(sampled_direction, surface_normal) < 0.0) {
    return std::nullopt;
  }

  auto* reflector = allocator.Scale(&reflector_, M_1_PI);
  auto pdf = PositiveDotProduct(sampled_direction, shading_normal);

  return Bsdf::Sample{*reflector, sampled_direction, /*transmitted=*/false,
                      /*specular=*/false, pdf};
}

const Reflector* LambertianBrdf::ReflectanceAll(
    const Vector& incoming, const Vector& surface_normal,
    const Vector& shading_normal, const Vector& outgoing, bool transmitted,
    SpectralAllocator& allocator, visual_t* pdf) const {
  return ReflectanceDiffuse(incoming, surface_normal, shading_normal, outgoing,
                            transmitted, allocator, pdf);
}

const Reflector* LambertianBrdf::ReflectanceDiffuse(
    const Vector& incoming, const Vector& surface_normal,
    const Vector& shading_normal, const Vector& outgoing, bool transmitted,
    SpectralAllocator& allocator, visual_t* pdf) const {
  if (transmitted) {
    return nullptr;
  }

  if (pdf) {
    *pdf = PositiveDotProduct(outgoing, shading_normal);
  }

  return allocator.Scale(&reflector_, M_1_PI);
}

}  // namespace bsdfs
}  // namespace iris