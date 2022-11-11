#ifndef _IRIS_TESTING_LIGHT_SAMPLER_
#define _IRIS_TESTING_LIGHT_SAMPLER_

#include <functional>
#include <span>

#include "iris/light_sampler.h"

namespace iris {
namespace testing {

LightSampler& GetEmptyLightSampler();

struct LightSampleListEntry {
  const Light* light;
  const std::optional<visual_t> pdf;
};

void ScopedListLightSampler(std::span<const LightSampleListEntry> sample_list,
                            std::function<void(LightSampler&)> callback);

}  // namespace testing
}  // namespace iris

#endif  // _IRIS_TESTING_LIGHT_SAMPLER_