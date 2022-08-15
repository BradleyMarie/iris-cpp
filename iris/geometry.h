#ifndef _IRIS_GEOMETRY_
#define _IRIS_GEOMETRY_

#include <optional>

#include "iris/float.h"
#include "iris/hit_allocator.h"
#include "iris/ray.h"

namespace iris {

class Geometry {
 public:
  Hit* Trace(HitAllocator& hit_allocator,
             std::optional<geometric_t> minimum_distance = std::nullopt,
             std::optional<geometric_t> maximum_distance = std::nullopt) const {
    return Trace(hit_allocator.ray_, hit_allocator, minimum_distance,
                 maximum_distance);
  }

 private:
  virtual Hit* Trace(const Ray& ray, HitAllocator& hit_allocator,
                     std::optional<geometric_t> minimum_distance,
                     std::optional<geometric_t> maximum_distance) const = 0;
};

}  // namespace iris

#endif  // _IRIS_GEOMETRY_