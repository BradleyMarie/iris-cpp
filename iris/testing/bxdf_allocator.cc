#include "iris/testing/bxdf_allocator.h"

#include "iris/internal/arena.h"

namespace iris {
namespace testing {

BxdfAllocator& GetBxdfAllocator() {
  thread_local internal::Arena arena;
  thread_local BxdfAllocator bxdf_allocator(arena);
  return bxdf_allocator;
}

}  // namespace testing
}  // namespace iris