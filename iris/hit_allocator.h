#ifndef _IRIS_HIT_ALLOCATOR_
#define _IRIS_HIT_ALLOCATOR_

#include <cstddef>
#include <type_traits>

#include "iris/float.h"
#include "iris/hit.h"
#include "iris/integer.h"
#include "iris/ray.h"

namespace iris {
namespace internal {
class HitArena;
}  // namespace internal

class HitAllocator final {
 public:
  HitAllocator(const Ray& ray, internal::HitArena& arena)
      : ray_(ray), arena_(arena) {}

  Hit& Allocate(Hit* next, geometric distance, face_t front, face_t back) {
    return Allocate(next, distance, front, back, nullptr, 0);
  }

  template <class T>
  requires(std::is_trivially_destructible<T>::value) Hit& Allocate(
      Hit* next, geometric distance, face_t front, face_t back,
      const T& additional_data) {
    return Allocate(next, distance, front, back, &additional_data, sizeof(T));
  }

 private:
  HitAllocator(const HitAllocator&) = delete;
  HitAllocator& operator=(const HitAllocator&) = delete;

  Hit& Allocate(Hit* next, geometric distance, face_t front, face_t back,
                const void* additional_data, size_t additional_data_size);

  const Ray& ray_;
  internal::HitArena& arena_;

  friend class Geometry;
};

}  // namespace iris

#endif  // _IRIS_HIT_ALLOCATOR_