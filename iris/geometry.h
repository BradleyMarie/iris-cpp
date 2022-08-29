#ifndef _IRIS_GEOMETRY_
#define _IRIS_GEOMETRY_

#include <optional>
#include <span>
#include <variant>

#include "iris/emissive_material.h"
#include "iris/hit_allocator.h"
#include "iris/integer.h"
#include "iris/material.h"
#include "iris/normal_map.h"
#include "iris/point.h"
#include "iris/random.h"
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

  virtual std::variant<Vector, const NormalMap*> ComputeShadingNormal(
      face_t face, const void* additional_data) const;

  virtual const Material* GetMaterial(face_t face,
                                      const void* additional_data) const;

  virtual bool IsEmissive(face_t face) const;

  virtual const EmissiveMaterial* GetEmissiveMaterial(
      face_t face, const void* additional_data) const;

  virtual std::optional<Point> SampleFace(face_t face, Random& rng) const;

  virtual std::optional<visual_t> ComputeArea(face_t face) const;

  virtual std::span<const face_t> GetFaces() const = 0;

 private:
  virtual Hit* Trace(const Ray& ray, HitAllocator& hit_allocator) const = 0;
};

}  // namespace iris

#endif  // _IRIS_GEOMETRY_