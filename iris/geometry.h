#ifndef _IRIS_GEOMETRY_
#define _IRIS_GEOMETRY_

#include <optional>
#include <variant>

#include "iris/hit_allocator.h"
#include "iris/integer.h"
#include "iris/normal_map.h"
#include "iris/point.h"
#include "iris/ray.h"
#include "iris/texture_coordinates.h"
#include "iris/vector.h"

namespace iris {

class Geometry {
 public:
  Hit* Trace(HitAllocator& hit_allocator) const;

  virtual Vector ComputeSurfaceNormal(const Point& hit_point, face_t face,
                                      const void* additional_data) const = 0;

  virtual std::optional<TextureCoordinates> ComputeTextureCoordinates(
      const Point& hit_point, face_t face, const void* additional_data) const;

  virtual std::variant<Vector, NormalMap*> ComputeShadingNormal(
      face_t face, const void* additional_data) const;

 private:
  virtual Hit* Trace(const Ray& ray, HitAllocator& hit_allocator) const = 0;
};

}  // namespace iris

#endif  // _IRIS_GEOMETRY_