#include "iris/light_sample_allocator.h"

#include "iris/internal/arena.h"

namespace iris {

LightSample& LightSampleAllocator::Allocate(const Light& light,
                                            const std::optional<visual_t>& pdf,
                                            LightSample* next) {
  return arena_.Allocate<LightSample>(next, light, pdf);
}

}  // namespace iris