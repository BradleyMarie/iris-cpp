#include "iris/bxdfs/lambertian_brdf.h"

#define _USE_MATH_CONSTANTS
#include <cassert>
#include <cmath>

namespace iris {
namespace bxdfs {

Vector LambertianBrdf::Sample(const Vector& incoming, Random& rng) const {
  auto radius_squared = rng.NextGeometric();
  auto radius = std::sqrt(radius_squared);

  auto theta = rng.NextGeometric() * static_cast<geometric>(2.0 * M_PI);
  auto sin_theta = std::sin(theta);
  auto cos_theta = std::cos(theta);

  auto x = radius * cos_theta;
  auto y = radius * sin_theta;
  auto z = std::sqrt(static_cast<geometric>(1.0) - radius_squared);

  return Vector(x, y, std::copysign(z, -incoming.z));
}

std::optional<visual_t> LambertianBrdf::Pdf(const Vector& incoming,
                                            const Vector& outgoing,
                                            SampleSource sample_source) const {
  return (incoming.z < 0) == (outgoing.z < 0) ? 0.0 : std::abs(outgoing.z);
}

const Reflector* LambertianBrdf::Reflectance(
    const Vector& incoming, const Vector& outgoing, SampleSource sample_source,
    Hemisphere hemisphere, SpectralAllocator& allocator) const {
  if (hemisphere != Hemisphere::BRDF) {
    return nullptr;
  }

  if ((incoming.z < 0) == (outgoing.z < 0)) {
    return nullptr;
  }

  return allocator.Scale(&reflector_, M_1_PI);
}

}  // namespace bxdfs
}  // namespace iris