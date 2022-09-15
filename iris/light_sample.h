#ifndef _IRIS_LIGHT_SAMPLE_
#define _IRIS_LIGHT_SAMPLE_

#include <optional>

#include "iris/float.h"
#include "iris/light.h"

namespace iris {

struct LightSample final {
  LightSample* next;
  const Light& light;
  const std::optional<visual_t> pdf;
};

}  // namespace iris

#endif  // _IRIS_LIGHT_SAMPLE_