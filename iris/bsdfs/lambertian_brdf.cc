#include "iris/bsdfs/lambertian_brdf.h"

#define _USE_MATH_CONSTANTS
#include <cmath>
#include <random>

namespace iris {
namespace bsdfs {
namespace {

Vector Orthogonal(const Vector& vector) {
  geometric values[] = {0.0, 0.0, 0.0};
  values[vector.DiminishedAxis()] = 1.0;
  return CrossProduct(vector, Vector(values[0], values[1], values[2]));
}

Vector TransformVector(const Vector& surface_normal, const Vector& vector) {
  Vector orthogonal = Orthogonal(surface_normal);
  Vector cross_product = CrossProduct(surface_normal, orthogonal);

  auto x = orthogonal.x * vector.x + cross_product.x * vector.y +
           surface_normal.x * vector.z;

  auto y = orthogonal.y * vector.x + cross_product.y * vector.y +
           surface_normal.y * vector.z;

  auto z = orthogonal.z * vector.x + cross_product.z * vector.y +
           surface_normal.z * vector.z;

  return Vector(x, y, z);
}

Vector SampleHemisphereWithCosineWeighting(const Vector& surface_normal,
                                           Random& rng) {
  std::uniform_real_distribution<geometric> distribution;

  auto radius_squared = distribution(rng);
  auto radius = std::sqrt(radius_squared);

  auto theta = distribution(rng) * 2.0 * M_PI - M_PI;
  auto sin_theta = std::sin(theta);
  auto cos_theta = std::cos(theta);

  float_t x = radius * cos_theta;
  float_t y = radius * sin_theta;

  return TransformVector(surface_normal,
                         Vector(x, y, std::sqrt(1.0 - radius_squared)));
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

  auto dot_product = DotProduct(sampled_direction, shading_normal);
  auto pdf =
      std::max(static_cast<visual_t>(0.0), static_cast<visual_t>(dot_product));

  return Bsdf::Sample{*reflector, sampled_direction, /*transmitted=*/false,
                      /*specular=*/false, pdf};
}

const Reflector* LambertianBrdf::ReflectanceAll(
    const Vector& incoming, const Vector& surface_normal,
    const Vector& shading_normal, const Vector& outgoing, bool transmitted,
    SpectralAllocator& allocator, visual_t* pdf = nullptr) const {
  return ReflectanceDiffuse(incoming, surface_normal, shading_normal, outgoing,
                            transmitted, allocator, pdf);
}

const Reflector* LambertianBrdf::ReflectanceDiffuse(
    const Vector& incoming, const Vector& surface_normal,
    const Vector& shading_normal, const Vector& outgoing, bool transmitted,
    SpectralAllocator& allocator, visual_t* pdf = nullptr) const {
  if (transmitted) {
    return nullptr;
  }

  if (pdf) {
    auto dot_product = DotProduct(outgoing, shading_normal);
    *pdf = std::max(static_cast<visual_t>(0.0),
                    static_cast<visual_t>(dot_product));
  }

  return allocator.Scale(&reflector_, M_1_PI);
}

}  // namespace bsdfs
}  // namespace iris