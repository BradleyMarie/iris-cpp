#include "iris/bxdf_allocator.h"

#include "iris/internal/arena.h"

namespace iris {

void* BxdfAllocator::Allocate(size_t size) { return arena_.Allocate(size); }

}  // namespace iris