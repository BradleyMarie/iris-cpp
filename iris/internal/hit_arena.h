#ifndef _IRIS_INTERNAL_HIT_ARENA_
#define _IRIS_INTERNAL_HIT_ARENA_

#include <memory>
#include <vector>

#include "iris/internal/arena.h"
#include "iris/internal/hit.h"

namespace iris::internal {

class HitArena final {
 public:
  Hit& Allocate(iris::Hit* next, geometric_t distance, face_t front,
                face_t back, const void* additional_data,
                size_t additional_data_size);
  void Clear();

 private:
  Arena arena_;
};

}  // namespace iris::internal

#endif  // _IRIS_INTERNAL_HIT_ARENA_