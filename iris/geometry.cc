#include "iris/geometry.h"

#include "iris/internal/hit.h"

namespace iris {

Hit* Geometry::Trace(HitAllocator& hit_allocator) const {
  auto* result = Trace(hit_allocator.ray_, hit_allocator);

  for (auto* hit_list = result; hit_list; hit_list = hit_list->next) {
    auto* full_hit = static_cast<internal::Hit*>(hit_list);
    if (!full_hit->geometry) {
      full_hit->geometry = this;
    }
  }

  return result;
}

std::optional<TextureCoordinates> Geometry::ComputeTextureCoordinates(
    const Point& hit_point, face_t face, const void* additional_data) const {
  return std::nullopt;
}

std::variant<Vector, const NormalMap*> Geometry::ComputeShadingNormal(
    face_t face, const void* additional_data) const {
  return nullptr;
}

const Material* Geometry::GetMaterial(face_t face,
                                      const void* additional_data) const {
  return nullptr;
}

const EmissiveMaterial* Geometry::GetEmissiveMaterial(
    face_t face, const void* additional_data) const {
  return nullptr;
}

bool Geometry::IsEmissive(face_t face) const { return false; }

std::optional<Point> Geometry::SampleFace(face_t face, Sampler& sampler) const {
  return std::nullopt;
}

std::optional<visual_t> Geometry::ComputeArea(face_t face) const {
  return std::nullopt;
}

}  // namespace iris