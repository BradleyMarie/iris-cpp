#ifndef _IRIS_TESTING_HIT_ALLOCATOR_
#define _IRIS_TESTING_HIT_ALLOCATOR_

#include <memory>

#include "iris/hit_allocator.h"

namespace iris {
namespace testing {

HitAllocator MakeHitAllocator(const Ray& ray);

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_HIT_ALLOCATOR_