#include "iris/internal/hit_arena.h"

#include <cstring>

namespace iris::internal {

Hit& HitArena::Allocate(iris::Hit* next, geometric distance, face_t front,
                        face_t back, const void* additional_data,
                        size_t additional_data_size) {
  auto* allocated_additional_data = arena_.Allocate(additional_data_size);
  memcpy(allocated_additional_data, additional_data, additional_data_size);

  return arena_.Allocate<Hit>(next, distance, front, back,
                              allocated_additional_data);
}

void HitArena::Clear() { arena_.Clear(); }

}  // namespace iris::internal