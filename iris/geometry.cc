#include "iris/geometry.h"

#include "iris/internal/hit.h"

namespace iris {

Hit* Geometry::Trace(HitAllocator& hit_allocator) const {
  auto* result = Trace(hit_allocator.ray_, hit_allocator);

  auto* hit_list = result;
  while (hit_list) {
    auto* full_hit = static_cast<internal::Hit*>(hit_list);
    if (!full_hit->geometry) {
      full_hit->geometry = this;
    }
    hit_list = hit_list->next;
  }

  return result;
}

std::optional<TextureCoordinates> Geometry::ComputeTextureCoordinates(
    const Point& hit_point, face_t face, const void* additional_data) const {
  return std::nullopt;
}

std::variant<Vector, NormalMap*> Geometry::ComputeShadingNormal(
    face_t face, const void* additional_data) const {
  return nullptr;
}

Material* Geometry::GetMaterial(face_t face,
                                const void* additional_data) const {
  return nullptr;
}

}  // namespace iris