#include "iris/bxdfs/lambertian_brdf.h"

#define _USE_MATH_CONSTANTS
#include <cassert>
#include <cmath>
#include <random>

namespace iris {
namespace bxdfs {

Vector LambertianBrdf::Sample(const Vector& incoming, Random& rng) const {
  std::uniform_real_distribution<geometric> distribution;

  auto radius_squared = distribution(rng);
  auto radius = std::sqrt(radius_squared);

  auto theta = distribution(rng) * static_cast<geometric>(2.0 * M_PI) -
               static_cast<geometric>(M_PI);
  auto sin_theta = std::sin(theta);
  auto cos_theta = std::cos(theta);

  auto x = radius * cos_theta;
  auto y = radius * sin_theta;
  auto z = std::sqrt(static_cast<geometric>(1.0) - radius_squared);

  return Vector(x, y, std::copysign(z, -incoming.z));
}

std::optional<visual_t> LambertianBrdf::Pdf(const Vector& incoming,
                                            const Vector& outgoing) const {
  return (incoming.z < 0) == (outgoing.z < 0) ? 0.0 : std::abs(outgoing.z);
}

const Reflector* LambertianBrdf::Reflectance(
    const Vector& incoming, const Vector& outgoing, Type type,
    SpectralAllocator& allocator) const {
  if (type != Bxdf::BRDF) {
    return nullptr;
  }

  if ((incoming.z < 0) == (outgoing.z < 0)) {
    return nullptr;
  }

  return allocator.Scale(&reflector_, M_1_PI);
}

}  // namespace bxdfs
}  // namespace iris