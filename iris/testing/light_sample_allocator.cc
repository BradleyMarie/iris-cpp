#include "iris/testing/light_sample_allocator.h"

#include "iris/internal/arena.h"

namespace iris {
namespace testing {

LightSampleAllocator& GetLightSampleAllocator() {
  thread_local internal::Arena arena;
  thread_local LightSampleAllocator light_sample_allocator(arena);
  return light_sample_allocator;
}

}  // namespace testing
}  // namespace iris