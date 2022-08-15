#ifndef _IRIS_INTERNAL_HIT_ARENA_
#define _IRIS_INTERNAL_HIT_ARENA_

#include <memory>
#include <vector>

#include "iris/internal/hit.h"

namespace iris::internal {

class HitArena {
 public:
  Hit& Allocate(iris::Hit* next, geometric_t distance, face_t front,
                face_t back, const void* additional_data,
                size_t additional_data_size);
  void Clear();

 private:
  std::vector<std::unique_ptr<Hit>> hits_;
  size_t hit_index_ = 0;

  std::vector<std::vector<char>> additional_data_;
  size_t additional_data_index_ = 0;
};

}  // namespace iris::internal

#endif  // _IRIS_INTERNAL_HIT_ARENA_