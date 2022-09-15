#ifndef _IRIS_LIGHT_SAMPLE_ALLOCATOR_
#define _IRIS_LIGHT_SAMPLE_ALLOCATOR_

#include <optional>

#include "iris/float.h"
#include "iris/light.h"
#include "iris/light_sample.h"

namespace iris {
namespace internal {
class Arena;
}  // namespace internal

class LightSampleAllocator final {
 public:
  LightSampleAllocator(internal::Arena& arena) noexcept : arena_(arena) {}

  LightSample& Allocate(const Light& light, const std::optional<visual_t>& pdf,
                        LightSample* next = nullptr);

 private:
  LightSampleAllocator(const LightSampleAllocator&) = delete;
  LightSampleAllocator& operator=(const LightSampleAllocator&) = delete;

  internal::Arena& arena_;
};

}  // namespace iris

#endif  // _IRIS_LIGHT_SAMPLE_ALLOCATOR_