#include "iris/hit_allocator.h"

#include <cassert>

#include "iris/internal/hit_arena.h"

namespace iris {

Hit& HitAllocator::Allocate(Hit* next, geometric distance, face_t front,
                            face_t back, const void* additional_data,
                            size_t additional_data_size) {
  assert(!next || distance < next->distance);
  return arena_.Allocate(next, distance, front, back, additional_data,
                         additional_data_size);
}

}  // namespace iris