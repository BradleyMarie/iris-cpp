#include "iris/bsdf.h"

#include <array>

namespace iris {
namespace {

std::pair<Vector, Vector> CreateLocalCoordinateSpace(
    const Vector& surface_normal) {
  geometric values[] = {0.0, 0.0, 0.0};
  values[surface_normal.DiminishedAxis()] = 1.0;
  Vector orthogonal0 =
      CrossProduct(surface_normal, Vector(values[0], values[1], values[2]));
  Vector orthogonal1 = CrossProduct(orthogonal0, surface_normal);
  return std::make_pair(orthogonal1, orthogonal0);
}

std::pair<Vector, Vector> Normalize(const std::pair<Vector, Vector>& vectors) {
  return std::make_pair(Normalize(vectors.first), Normalize(vectors.second));
}

std::pair<Vector, Vector> MaybeNormalize(const Vector& surface_normal,
                                         const Vector& shading_normal,
                                         bool normalize) {
  auto vectors = std::make_pair(surface_normal, shading_normal);
  return normalize ? Normalize(vectors) : vectors;
}

std::array<Vector, 4> ComputeState(const Vector& surface_normal,
                                   const Vector& shading_normal,
                                   bool normalize) {
  auto vectors = MaybeNormalize(surface_normal, shading_normal, normalize);
  auto coordinate_space = CreateLocalCoordinateSpace(vectors.second);
  return {coordinate_space.first, coordinate_space.second, vectors.second,
          vectors.first};
}

}  // namespace

Bsdf::Bsdf(const Bxdf& bxdf, const Vector& surface_normal,
           const Vector& shading_normal, bool normalize) noexcept
    : Bsdf(bxdf,
           ComputeState(surface_normal, shading_normal, normalize).data()) {}

Vector Bsdf::ToLocal(const Vector& vector) const {
  return Vector(DotProduct(x_, vector), DotProduct(y_, vector),
                DotProduct(z_, vector));
}

Vector Bsdf::ToWorld(const Vector& vector) const {
  return Vector(x_.x * vector.x + y_.x * vector.y + z_.x * vector.z,
                x_.y * vector.x + y_.y * vector.y + z_.y * vector.z,
                x_.z * vector.x + y_.z * vector.y + z_.z * vector.z);
}

Bsdf::SampleResult Bsdf::Sample(const Vector& incoming, Random& rng,
                                SpectralAllocator& allocator) const {
  auto sample = bxdf_.Sample(ToLocal(incoming), rng, allocator);
  return {sample.reflector, ToWorld(sample.direction), sample.pdf};
}

const Reflector* Bsdf::Reflectance(const Vector& incoming,
                                   const Vector& outgoing,
                                   SpectralAllocator& allocator,
                                   visual_t* pdf) const {
  bool transmitted = (DotProduct(incoming, surface_normal_) > 0) ==
                     (DotProduct(outgoing, surface_normal_) > 0);
  return bxdf_.Reflectance(ToLocal(incoming), ToLocal(outgoing),
                           transmitted ? Bxdf::BTDF : Bxdf::BRDF, allocator,
                           pdf);
}

}  // namespace iris