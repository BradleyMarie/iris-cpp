#include "iris/testing/hit_allocator.h"

#include <vector>

#include "iris/internal/hit_arena.h"

namespace iris {
namespace testing {

HitAllocator MakeHitAllocator(const Ray& ray) {
  static std::vector<std::unique_ptr<Ray>> rays;
  static internal::HitArena arena;

  rays.push_back(std::make_unique<Ray>(ray));

  return HitAllocator(*rays.back(), arena);
}

}  // namespace testing
}  // namespace iris